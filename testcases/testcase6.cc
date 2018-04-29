#include "cache.h"
#include <iostream>
#include <stdlib.h>
#include <sstream>

#define KB 1024

using namespace std;

/* Test case for cache simulator */ 

/* DO NOT MODIFY */

int main(int argc, char **argv){

	cache *mycache = NULL;

	for (unsigned b=1024 * 16; b<=512 * 1024; b=b*2){

	cout << "CACHE SIZE = " << dec << b / 1024 << "KB" << endl;
	cout << "===================" << endl << endl;

	mycache = new cache(     b,			//size
				  2,			//associativity
				  256,			//cache line size
				  WRITE_BACK,		//write hit policy
				  WRITE_ALLOCATE, 	//write miss policy
				  5, 			//hit time
				  100, 			//miss penalty
				  48    		//address width
				  );     

	mycache->print_configuration();

	mycache->load_trace("traces/GCC.t");

	mycache->run();	

	cout << endl;

	mycache->print_statistics();

	cout << endl;
	
	delete mycache;

	}

}