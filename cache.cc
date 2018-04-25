#include "cache.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <iomanip>

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
}

void cache::print_configuration(){
	cout << "CACHE INFORMATION" << endl;
	cout << "size = " << size << " KB" << endl;
	cout << "associativity = " << associativity << "-way" << endl;
	cout << "cache line size = " << line_size << " B" << endl;
	cout << "write hit policy = " << write_policy_t_str.at(wr_hit_policy) << endl;
	cout << "write miss policy = " << write_policy_t_str.at(wr_miss_policy) << endl;
	cout << "cache hit time = " << hit_time << " CLK" << endl;
	cout << "cache miss penalty = " << miss_penalty << " CLK" << endl;
	cout << "memory address width = " << address_width << " bits" << endl;
}

cache::~cache(){
	/* edit here */
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

	number_memory_accesses++;
	if (num_entries!=0 && (number_memory_accesses-first_access)==num_entries)
		break;
   }
}

void cache::print_statistics(){
	cout << "STATISTICS" << endl;
	/* edit here */
}

access_type_t cache::read(address_t address){
	/* edit here */
	return MISS;
}

access_type_t cache::write(address_t address){
	/* edit here */
        return MISS;
}

void cache::print_tag_array(){
	cout << "TAG ARRAY" << endl;
	/* edit here */
}

unsigned cache::evict(unsigned index){
	/* edit here */
	return 0;
}

