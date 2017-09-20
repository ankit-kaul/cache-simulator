#include "cachesim.hpp"
#include <cstdio>
#include <math.h>
#include <stdio.h>
#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <vector>
#include <iostream>  
#include <bitset>
#include <tuple>
#include <algorithm>
#include <inttypes.h>

using namespace std;

/**
 * XXX: You are welcome to define and set any global classes and variables as needed.
 */

 std::uint64_t n_Lines=0;
 int n_offset_bits=0;
 int n_sets=0;
 int n_index_bits=0;
 int n_tag_bits=0;
 int n_LruCounter_bits=0;
 int Total_size;
 std::uint64_t Associativity=0;
 int Block_size;
 int invalid_ctr=0;
 int LRU_index=0;
 
 int LRU_counter_value=0;
 int hit_flag=0;
 int readHit_ctr=0;
 int writeHit_ctr=0;
 int access_ctr=0;
 int read_ctr=0;
 int write_ctr=0;
 int current_used_index=0;
 int current_used_ctr_value=0;
 bool block_evict_flag=0;
 double decml;
 int hit_timer;
 
 
 std::uint64_t addr_tag;
 std::uint64_t addr_block; //Variable holding Tag and Index together
 std::uint64_t addr_index;
 
 //std::uint64_t offset;
 
 std::uint64_t i=0, k=0;
  
 //Valid bit, Dirty bit, Tag, LRU counter
 std::vector< std::vector<std::tuple <bool, bool, std::uint64_t, int> > >metaData;
 std::vector<std::tuple <bool, bool, std::uint64_t, int> > tempSet;
 //std::tuple <bool, bool, std::uint64_t, int> tempLine;
 
 std::vector<std::tuple <bool, bool, std::uint64_t, int> > tempSetVect;
 std::tuple <bool, bool, std::uint64_t, int>  tempLineVect;
 
 //Memory Vector
 std::vector <std::uint64_t> addrVector;
 std::uint64_t comp_misses;
 
 
 
 
 //Shadow cache
 std::vector< std::vector<std::tuple <bool, bool, std::uint64_t, int> > >metaData_sc;
 std::vector<std::tuple <bool, bool, std::uint64_t, int> > tempSet_sc;
 
 std::vector<std::tuple <bool, bool, std::uint64_t, int> > tempSetVect_sc;
 std::tuple <bool, bool, std::uint64_t, int>  tempLineVect_sc;
 
 
 //SC Variables
 int invalid_ctr_sc=0;
 int LRU_index_sc=0;
 int LRU_counter_value_sc=0;
 int hit_flag_sc=0;
 int current_used_index_sc=0;
 int current_used_ctr_value_sc=0;
 int misses_sc=0;
 int write_misses_sc=0;
 int read_requests_sc = 0;
 int write_requests_sc =0;
 bool comp_miss_flag;
 bool capacity_miss_flag;
 bool conflict_miss_flag;
 bool block_evict_flag_sc=0;
 bool comp_miss_flag_sc;
 
 
 std::uint64_t addr_tag_sc;
 
 //Memory Vector
 std::vector <std::uint64_t> addrVector_sc;
 std::uint64_t comp_misses_sc;
 
 
 
/**
 * Subroutine for initializing the cache.
 * You may initalize any global or heap variables as needed.
 * XXX You're responsible for completing this routine.
 *
 * @param[in]   total_size      The total size of the cache in bytes (exluding metadata)
 * @param[in]   block_size      The size of each block in bytes
 * @param[in]   associativity   The set associativity of the cache
 */
void cache_setup(int total_size, int block_size, int associativity) {
	
	Total_size = total_size;
	Block_size = block_size;
	Associativity = associativity;
	
	//printf("\nThis is a discovery");
	
	n_Lines = (int)(total_size/block_size);
	n_offset_bits = (int)(log2f(block_size));
	
	//printf("\nOffset bits: %d", (int)n_offset_bits);
	
	n_sets = (int)(n_Lines/Associativity);
	n_index_bits = (int)(log2f(n_sets));
	
	//printf("\nIndex bits: %d", (int)n_index_bits);
	
	n_LruCounter_bits = (int)(log2f(Associativity));

	//Initialize the Line Vector with default values
	tempSet.assign(1,std::make_tuple(0,0,0,0));
	for(i=1; i<Associativity; i++) {		
		tempSet.push_back(std::make_tuple(0,0,0,i)); 
		/* Observation: Shouldn't use unnecessary brackets, e.g.: 
		Using ((0,0,0,i)) instead of (0,0,0,i) will give an error here. The outer brackets evaluate the inslde 
		arguments and retur an integer value */
		
	}
	
	metaData.assign (n_sets,tempSet);
	
	n_tag_bits = 64 - n_offset_bits - n_index_bits;
	
	/*
	printf("\nVector of Cache Meta Data : \n");

	for (i=0; i < n_sets; i++) {
		
		tempLineVect = metaData.at(i);
			
		for ( const auto& k : tempLineVect ) {
		cout << get<0>(k) << get<1>(k) << get<2>(k) << get<3>(k) << endl;
		}
	}
	
	printf("\n");
	//*/
	
	//Setup Shadow Cache
	
	//Initialize the Line Vector with default values
	tempSet_sc.assign(1,std::make_tuple(0,0,0,0));
	for(i=1; i<n_Lines; i++) {		
		tempSet_sc.push_back(std::make_tuple(0,0,0,i)); 
		/* Observation: Shouldn't use unnecessary brackets, e.g.: 
		Using ((0,0,0,i)) instead of (0,0,0,i) will give an error here. The outer brackets evaluate the inslde 
		arguments and retur an integer value */
		
	}
	
	metaData_sc.assign (1,tempSet_sc);
	//printf("\nUser cache sets %d, lines per set %d \n", metaData.size(),metaData.at(0).size());
	//printf("Shadow cache sets %d, lines per set %d\n", metaData_sc.size(),metaData_sc.at(0).size());
}

/**
 * Subroutine for doing a single access to the cache.
 * XXX You're responsible for completing this routine.
 *
 * @param[in]   rw              Either MEMOP_READ or MEMOP_WRITE
 * @param[in]   address         The address in 64-bits
 * @param[out]  p_stats         Update any statistics as needed
 */
void cache_access(memop_type rw, std::uint64_t address, cache_stats_t* p_stats) {
	
	///*
	invalid_ctr = 0;
	LRU_index = 0;
	current_used_index = 0;
	hit_flag = 0;
	capacity_miss_flag = 0;
	conflict_miss_flag = 0;
	block_evict_flag = 0;
	
	//Updating Shadow Cache Variables
	invalid_ctr_sc = 0;
	LRU_index_sc = 0;
	current_used_index_sc = 0;
	hit_flag_sc = 0;
	block_evict_flag_sc = 0;
	
	
	
	if(rw == 'r'){ //If it's a Read Operation
		p_stats->reads = p_stats->reads + 1;
	}

	else {                //If it's a Write Operation 
		p_stats->writes = p_stats->writes + 1;
		
	}
	
	p_stats->accesses = p_stats->accesses + 1;
	
	//printf("\nAddress: %" PRIx64 "\n", address);
	//printf("Address in binary: ");
	//binary(address);
	
	addr_tag = address >> (n_index_bits + n_offset_bits);//Extract upper x bits from address to get the tag
	// printf("\nTag for this instruction: %" PRIx64 "\n", addr_tag);
	//printf("Tag for this instruction: ");
	// binary(addr_tag);
	
	addr_block = address >> (n_offset_bits);//Remove lower offset bits from address
	
	//printf("TagIndex for this instruction: ");
	//printf("TagIndex for this instruction: %" PRIx64 "\n", addr_block);
	
	addr_index = addr_block & ((1<<n_index_bits) - 1); //Extract lower x bits from address to get the index
	//printf("Index for this instruction: %" PRIx64 "\n", addr_index);
	//printf("Index for this instruction: ");
	
	
	//Extract separate Tag for the Shadow Cache
	addr_tag_sc = address >> (n_offset_bits);//Extract upper x bits from address to get the tag
	
	
	if(n_sets > 1){ //Number of sets > 1
		tempSetVect = metaData.at(addr_index);
		
	}
	else{ //Number of sets = 1, Fully associative cache
		tempSetVect = metaData.at(0);
		
	}
	
	//Since the Shadow cace will always have just one set, assign the first and only element to the tempSetVect_sc
	tempSetVect_sc = metaData_sc.at(0);
	
	///*
	
	//CHECK FOR A COMPULSORY MISS
	comp_miss_flag = 1;
	
	for (i=0; i<comp_misses; i++){ //Loop through the Address Vector
	
		if(addrVector.at(i) == addr_block) {
			comp_miss_flag = 0;
			
		}
	}
	
	if(comp_miss_flag) {
		comp_misses++;
		addrVector.push_back(addr_block); 
	}
	
	
	
	//CHECK FOR HIT OR MISS
	for (i=0; i<Associativity; i++){ //Loop through the sets
		
		
		if(std::get<0>(tempSetVect.at(i)) == 1 && addr_tag == std::get<2>(tempSetVect.at(i))){ //If Valid bit = 1 (Data valid) and Input Tag matches a tag in the set
			
			current_used_index = i;
			hit_flag = 1;
			//printf("Hit\n");
			break;
	
		}
		else {
			
			//Find LRU line
			if(std::get<3>(tempSetVect.at(i)) == 0){
			LRU_index = i;
			}
			
		}//end of else
		
	}//end of for
	
	
	if(hit_flag == 1) {										//HIT
		
		//printf("Cache Hit %d\t", current_used_index);
		//EXTRACT THE CURRENT POSITION VALUE
		current_used_ctr_value = std::get<3>(tempSetVect.at(current_used_index));
		
		//Update LRU counters
		for (i=0; i<Associativity; i++){ //Looping through current Set
			
			if(std::get<3>(tempSetVect.at(i)) > current_used_ctr_value){
				std::get<3>(tempSetVect.at(i))--;
			}
			
		}
		
		std::get<3>(tempSetVect.at(current_used_index)) = Associativity-1;
		
		if(rw == 'w'){ //If it's a Write Operation
			std::get<1>(tempSetVect.at(current_used_index)) = 1; //Setting Dirty bit to 1
		}
	}
	
	else { 													//MISS
	
		//printf("Cache miss %d\t", LRU_index);
		p_stats->misses = p_stats->misses + 1;
		p_stats->read_requests = p_stats->read_requests + 1;

		
		if(std::get<0>(tempSetVect.at(LRU_index)) == 1 && std::get<1>(tempSetVect.at(LRU_index)) == 1) {
			
			p_stats->write_requests = p_stats->write_requests + 1;
			block_evict_flag = 1;
			
		}
		
		LRU_counter_value = std::get<3>(tempSetVect.at(LRU_index));
		
		std::get<0>(tempSetVect.at(LRU_index)) = 1; //Setting Valid bit to 1
		std::get<2>(tempSetVect.at(LRU_index)) = addr_tag; //Setting Tag to input tag
		
		//Update LRU counters
		for (i=0; i<Associativity; i++){ //Looping through current Set
			
			if(std::get<3>(tempSetVect.at(i)) > LRU_counter_value){
				std::get<3>(tempSetVect.at(i))--;
			}
			
		}
		std::get<3>(tempSetVect.at(LRU_index)) = Associativity-1;
		
		if(rw == 'w'){ //If it's a Write Operation
			
			std::get<1>(tempSetVect.at(LRU_index)) = 1; //Setting Dirty bit to 1
			p_stats->write_misses = p_stats->write_misses + 1;
		}
		
		else { 			//If it's a Read Operation
			
			std::get<1>(tempSetVect.at(LRU_index)) = 0; //Setting Dirty bit to 0
			
		}
		
	}
	
	
	//*/
	
	//SHADOW CACHE UPDATE
	
	
	//CHECK FOR A COMPULSORY MISS
	comp_miss_flag_sc = 1;
	
	for (i=0; i<comp_misses_sc; i++){ //Loop through the Address Vector
	
		if(addrVector_sc.at(i) == addr_block) {
			comp_miss_flag_sc = 0;
			
		}
	}
	
	if(comp_miss_flag_sc) {
		comp_misses_sc++;
		addrVector_sc.push_back(addr_block); 
	}
	
	
	//CHECK FOR HIT OR MISS
	
	
	
	for (i=0; i<n_Lines; i++){ //Loop through the sets
		
		
		if(std::get<0>(tempSetVect_sc.at(i)) == 1 && addr_tag_sc == std::get<2>(tempSetVect_sc.at(i))){ //If Valid bit = 1 (Data valid) and Input Tag matches a tag in the set
			
			current_used_index_sc = i;
			hit_flag_sc = 1;
			break;
	
		}
		else {
			
			//Find LRU line
			if(std::get<3>(tempSetVect_sc.at(i)) == 0){
			LRU_index_sc = i;
			}
			
		}//end of else
		
	}//end of for
	
	
	if(hit_flag_sc == 1) {										//HIT
		
		//printf("Shadow Cache Hit %d\n", current_used_index_sc);
		//EXTRACT THE CURRENT POSITION VALUE
		current_used_ctr_value_sc = std::get<3>(tempSetVect_sc.at(current_used_index_sc));
		
		//Update LRU counters
		for (i=0; i<n_Lines; i++){ //Looping through current Set
			
			if(std::get<3>(tempSetVect_sc.at(i)) > current_used_ctr_value_sc){
				std::get<3>(tempSetVect_sc.at(i))--;
			}
			
		}
		
		std::get<3>(tempSetVect_sc.at(current_used_index_sc)) = n_Lines-1;
		
		if(rw == 'w'){ //If it's a Write Operation
			std::get<1>(tempSetVect_sc.at(current_used_index_sc)) = 1; //Setting Dirty bit to 1
		}
	}
	
	else { 													//MISS
		
		//p_stats->misses = p_stats->misses + 1;
		//p_stats->read_requests = p_stats->read_requests + 1;
		//printf("Shadow Cache Miss %d\n", LRU_index_sc);
		misses_sc = misses_sc + 1;
		read_requests_sc = read_requests_sc + 1;

		
		if(std::get<0>(tempSetVect_sc.at(LRU_index_sc)) == 1 && std::get<1>(tempSetVect_sc.at(LRU_index_sc)) == 1) { //If Valid and Dirty were set
			
			//p_stats->write_requests = p_stats->write_requests + 1;
			write_requests_sc = write_requests_sc + 1;
			block_evict_flag_sc = 1;
			
		}
		
		LRU_counter_value_sc = std::get<3>(tempSetVect_sc.at(LRU_index_sc));
		
		std::get<0>(tempSetVect_sc.at(LRU_index_sc)) = 1; //Setting Valid bit to 1
		std::get<2>(tempSetVect_sc.at(LRU_index_sc)) = addr_tag_sc; //Setting Tag to input tag
		
		//Update LRU counters
		for (i=0; i<n_Lines; i++){ //Looping through current Set
			
			if(std::get<3>(tempSetVect_sc.at(i)) > LRU_counter_value_sc){
				std::get<3>(tempSetVect_sc.at(i))--;
			}
			
		}
		std::get<3>(tempSetVect_sc.at(LRU_index_sc)) = n_Lines-1;
		
		if(rw == 'w'){ //If it's a Write Operation
			
			std::get<1>(tempSetVect_sc.at(LRU_index_sc)) = 1; //Setting Dirty bit to 1
			//p_stats->write_misses = p_stats->write_misses + 1;
			write_misses_sc = write_misses_sc + 1;
		}
		
		else { 			//If it's a Read Operation
			
			std::get<1>(tempSetVect_sc.at(LRU_index_sc)) = 0; //Setting Dirty bit to 0
			
		}
		
	}
	
	//Capacity MISS check, when a block is evicted in both our original cache (N-way associative) and a fully associative cache
	if(comp_miss_flag == 0 && hit_flag == 0  && ((hit_flag_sc == 0))) { //
		
		p_stats->capacity = p_stats->capacity + 1;
		capacity_miss_flag = 1;
		
	}
	
	//Conflict MISS check, when a block experiences neither a Compulsory nor a Capacity miss
	if(comp_miss_flag == 0 && hit_flag == 0 && capacity_miss_flag == 0 ) {
		
		p_stats->conflict = p_stats->conflict + 1;
		conflict_miss_flag = 1;
		
	}
	
	
	
	
	if(n_sets > 1){ //Number of sets > 1
		metaData.at(addr_index) = tempSetVect;
		//metaData_sc.at(addr_index) = tempSetVect_sc;
	}
	else{ //Number of sets = 1, Fully associative cache
		metaData.at(0) = tempSetVect;
		//metaData_sc.at(0) = tempSetVect_sc;
	}
	
	
	//Number of sets = 1, Fully associative cache
	metaData_sc.at(0) = tempSetVect_sc;
	
	
	
}


/**
 * Subroutine for finalizing any statistics and terminate the cache
 * XXX You're responsible for completing this routine.
 *
 * @param[out]  p_stats         Update any statistics as needed
 */
void cache_complete(cache_stats_t *p_stats) {
	
	/*
	decml = std::fmod( float(0.2 * Associativity * 10), float(int(0.2 * Associativity)));
	printf("Decimal: %f", decml);
	
	if(decml >= 0.5) {
		hit_timer = std::fmod((0.2 * Associativity * 10), (0.2 * Associativity)) + 1;
		printf("Hit time: %d", hit_timer);
		
	}
	else {
		hit_timer = std::fmod( (0.2 * Associativity * 10), (0.2 * Associativity));
		printf("Hit time: %d", hit_timer);
	}
	*/
	
	p_stats->requests = p_stats->read_requests + p_stats->write_requests;
	p_stats->compulsory = comp_misses;
	p_stats->hit_time = std::ceil(0.2 * Associativity );
	p_stats->miss_penalty = std::ceil(0.2*(Associativity) + 50 + 0.25*(Block_size));
    p_stats->miss_rate = double(p_stats->misses) / p_stats->accesses;
    p_stats->amat = p_stats->hit_time + (p_stats->miss_rate * p_stats->miss_penalty);
    p_stats->storage_overhead = n_Lines*(n_tag_bits + 2);
    p_stats->storage_overhead_ratio = double(p_stats->storage_overhead) / ( (n_Lines * (Block_size))*8);
	
}
