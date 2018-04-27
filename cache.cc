/*
	TO DO: finish write function


*/

#include "cache.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;


map<write_policy_t, string> create_policy_map()
{
	map<write_policy_t, string> m;
	m[WRITE_BACK] = "write-back";
	m[WRITE_THROUGH] = "write-through";
	m[WRITE_ALLOCATE] = "write-allocate";
	m[NO_WRITE_ALLOCATE] = "no-write-allocate";
	return m;
}
map<write_policy_t, string> write_policy_t_str = create_policy_map();




cache::cache(unsigned size, 
	unsigned associativity,
	unsigned line_size,
	write_policy_t wr_hit_policy,
	write_policy_t wr_miss_policy,
	unsigned hit_time,
	unsigned miss_penalty,
	unsigned address_width
){
	this->size = size;
	this->associativity = associativity;
	this->line_size = line_size;
	this->wr_hit_policy = wr_hit_policy;
	this->wr_miss_policy = wr_miss_policy;
	this->hit_time = hit_time;
	this->miss_penalty = miss_penalty;
	this->address_width = address_width;

	num_sets = size / (line_size * associativity);

	number_memory_accesses = 0;
	reads = 0;
	read_misses = 0;
	writes = 0;
	write_misses = 0;
	evictions = 0;
	memory_writes = 1; //there will be at least one because of final

	//build cache
	for (unsigned i = 0; i < num_sets; i++){
		set_array.push_back(new Set(associativity, line_size, wr_hit_policy, wr_miss_policy, hit_time, miss_penalty, &evictions, &memory_writes));
	}
}

void cache::print_configuration(){
	cout << "CACHE INFORMATION" << endl;
	cout << "size = " << size/1024 << " KB" << endl;
	cout << "associativity = " << associativity << "-way" << endl;
	cout << "cache line size = " << line_size << " B" << endl;
	cout << "write hit policy = " << write_policy_t_str.at(wr_hit_policy) << endl;
	cout << "write miss policy = " << write_policy_t_str.at(wr_miss_policy) << endl;
	cout << "cache hit time = " << hit_time << " CLK" << endl;
	cout << "cache miss penalty = " << miss_penalty << " CLK" << endl;
	cout << "memory address width = " << address_width << " bits" << endl;
}

cache::~cache(){
	for (vector<Set *>::iterator it = set_array.begin(); it != set_array.end(); ++it){
		delete (*it);
	}
	set_array.clear();
}

void cache::load_trace(const char *filename){
   stream.open(filename);
}

void cache::run(unsigned num_entries){

   unsigned first_access = number_memory_accesses;
   string line;
   unsigned line_nr=0;

   while (getline(stream,line)){

	line_nr++;
        char *str = const_cast<char*>(line.c_str());
	
        // tokenize the instruction
        char *op = strtok (str," ");
	char *addr = strtok (NULL, " ");
	address_t address = strtoul(addr, NULL, 16);

	/* 
		edit here:
		insert the code to process read and write operations
	   	using the read() and write() functions below

	*/
	switch (*op){
	case ('r') :
		reads++;
		if (read(address) == MISS) read_misses++;
		break;
	case ('w') :
		writes++;
		if (write(address) == MISS) write_misses++;
		break;
	default: break;
	}

	number_memory_accesses++;
	if (num_entries!=0 && (number_memory_accesses-first_access)==num_entries)
		break;
   }
}

void cache::print_statistics(){
	cout << "STATISTICS" << dec << endl;
	cout << "memory accesses = " << number_memory_accesses << endl;
	cout << "read = " << reads << endl;
	cout << "read misses = " << read_misses << endl;
	cout << "write = " << writes << endl;
	cout << "write misses = " << write_misses << endl;
	cout << "evictions = " << evictions << endl;
	cout << "memory writes = " << memory_writes << endl;
	cout << "average memory access time = " << ((float)hit_time + (((float)read_misses + (float)write_misses) / (float)number_memory_accesses)*(float)miss_penalty) << endl;
}

access_type_t cache::read(address_t address){
	unsigned block_offset_bit_size = ceil(log2(line_size));
	unsigned index_bit_size = ceil(log2(num_sets));
	long long tag = address >> (index_bit_size + block_offset_bit_size);

	//create index mask
	long long index_mask = 0;
	for (unsigned i = 0; i < index_bit_size; i++){
		index_mask <<= 1;
		index_mask += 1;
	}
	for (unsigned i = 0; i < block_offset_bit_size; i++){
		index_mask = index_mask << 1;
	}
	//get index
	unsigned index = (address & index_mask) >> block_offset_bit_size;

	return set_array[index]->read(tag);
}

access_type_t cache::write(address_t address){
		//get tag
		//get index
		//write to set at index
		
	unsigned block_offset_bit_size = ceil(log2(line_size));
	unsigned index_bit_size = ceil(log2(num_sets));
	long long tag = address >> (index_bit_size + block_offset_bit_size);

	//create index mask
	long long index_mask = 0;
	for (unsigned i = 0; i < index_bit_size; i++){
		index_mask <<= 1;
		index_mask += 1;
	}
	for (unsigned i = 0; i < block_offset_bit_size; i++){
		index_mask = index_mask << 1;
	}
	//get index
	unsigned index = (address & index_mask) >> block_offset_bit_size;

	return set_array[index]->write(tag);
}

void cache::print_tag_array(){
	unsigned block_offset_bit_size = ceil(log2(line_size));
	unsigned index_bit_size = ceil(log2(num_sets));
	unsigned tag_bits = address_width - index_bit_size - block_offset_bit_size;

	cout << "TAG ARRAY" << endl;

	for (unsigned i = 0; i < associativity; i++){
		cout << "BLOCKS " << i << endl;
		cout << setfill(' ') << setw(7) << "index" << setw(6) << "dirty" << setw(4 + tag_bits / 4)
			<< "tag" << endl;
		for (unsigned j = 0; j < num_sets; j++){
			if (set_array[j]->getBlockTag(i) != UNDEFINED)
				cout << setfill(' ') << setw(7) << j << setw(6) << set_array[j]->getBlockDirty(i) << hex << setw(4 + tag_bits / 4)
					<< "0x" <<  set_array[j]->getBlockTag(i) << endl;
		}
	}
}

unsigned cache::evict(unsigned index){
	/* edit here */
	return 0;
}




cache::Set::Set(unsigned associativity, unsigned line_size, write_policy_t wr_hit_policy,
	write_policy_t wr_miss_policy, unsigned hit_time, unsigned miss_penalty,
	unsigned * evictions, unsigned * memory_writes){

	this->line_size = line_size;
	this->associativity = associativity;
	this->wr_hit_policy = wr_hit_policy;
	this->wr_miss_policy = wr_miss_policy;
	this->hit_time = hit_time;
	this->miss_penalty = miss_penalty;

	this->evictions = evictions;
	this->memory_writes = memory_writes;

	precedent = new unsigned[associativity];
	fill_n(precedent, associativity, (unsigned) UNDEFINED);

	for (unsigned i = 0; i < associativity; i++){
		block_array.push_back(new Block);
		block_array[i]->dirty_bit = 0;
		block_array[i]->valid = 0;
		block_array[i]->tag = UNDEFINED;
	}
}

cache::Set::~Set(){
	for (vector<Block *>::iterator it = block_array.begin(); it != block_array.end(); ++it){
		delete (*it);
	}
	block_array.clear();
	delete[] precedent;
}

bool cache::Set::getBlockValid(unsigned pos){
	return block_array[pos]->valid;
}

bool cache::Set::getBlockDirty(unsigned pos){
	return block_array[pos]->dirty_bit;
}

long long cache::Set::getBlockTag(unsigned pos){
	return block_array[pos]->tag;
}

access_type_t cache::Set::write(unsigned search_tag){
	//for write back, allocate

	//search for tag in block array
	//if not found, put in empty block -> return miss
	//if no block empty, evict least block used and put there -> return miss
	//if tag found, update block -> return hit
	for (unsigned i = 0; i < associativity; i++){
		if (search_tag == block_array[i]->tag){ //update 
			block_array[i]->dirty_bit = true;
			updatePrecedent(i);
			return HIT;
		}
		else if (block_array[i]->tag == UNDEFINED){ //put in empty block
			block_array[i]->tag = search_tag;
			block_array[i]->dirty_bit = true;
			updatePrecedent(i);
			return MISS;
		}
	}

	//evict
	for (unsigned i = 0; i < associativity; i++) { //evict
		unsigned LRU = getLRU();
		block_array[LRU]->tag = search_tag;
		block_array[LRU]->dirty_bit = true;
		updatePrecedent(i);
		(*memory_writes)++;
		(*evictions)++;
		return MISS;
	}

	return MISS;
}

access_type_t cache::Set::read(unsigned search_tag){
	//for write-back, allocate
	for (unsigned i = 0; i < associativity; i++){
		if (search_tag == block_array[i]->tag){ //update 
			updatePrecedent(i);
			return HIT;
		}
		else if (block_array[i]->tag == UNDEFINED){ //put in empty block
			block_array[i]->tag = search_tag;
			block_array[i]->dirty_bit = false;
			updatePrecedent(i);
			return MISS;
		}
	}

	//evict
	for (unsigned i = 0; i < associativity; i++) { //evict
		unsigned LRU = getLRU();
		block_array[LRU]->tag = search_tag;
		block_array[LRU]->dirty_bit = false;
		updatePrecedent(i);
		(*evictions)++;
		return MISS;
	}
	return MISS;
}

void cache::Set::updatePrecedent(unsigned pos){
	for (unsigned i = 0; i < associativity; i++){
		if (precedent[i] != (unsigned)UNDEFINED) precedent[i]++;
	}
	precedent[pos] = 0;
}

unsigned cache::Set::getLRU(){
	unsigned LRU = precedent[0];
	for (unsigned i = 1; i < associativity; i++){
		if (precedent[i] != (unsigned) UNDEFINED)
			if (precedent[i] > LRU) LRU = precedent[i];
	}
	return LRU;
}

