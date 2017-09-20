#include <cstdio>
#include <cinttypes>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include "cachesim.hpp"

void print_help_and_exit() {
    printf("cachesim [OPTIONS] < traces/file.trace\n");
    printf("  -t [TOTAL_SIZE]\tTotal size in bytes\n");
    printf("  -b [BLOCK_SIZE]\tSize of each block in bytes\n");
    printf("  -s [ASSOCIATIVITY]\tAssociativity\n");
    printf("  -h\t\tThis helpful output\n");
    exit(0);
}

void print_statistics(cache_stats_t* p_stats);

int main(int argc, char* argv[]) {
    int opt;
    int total_size      = 1024;
    int block_size      = 16;
    int associativity   = 4;

    // Process arguments
    while(-1 != (opt = getopt(argc, argv, "c:b:s:w:h"))) {
        switch(opt) {
        case 'c':
            total_size = atoi(optarg);
            break;
        case 'b':
            block_size = atoi(optarg);
            break;
        case 's':
            associativity = atoi(optarg);
            break;
        case 'h':
            // Fall through
        default:
            print_help_and_exit();
            break;
        }
    }

    printf("Cache Settings\n");
    printf("Total Size: %d\n", total_size);
    printf("Block Size: %d\n", block_size);
    printf("Associativity: %d\n", associativity);
    printf("\n");

    // Do some setup
    cache_setup(total_size, block_size, associativity);

    cache_stats_t stats;
    memset(&stats, 0, sizeof(cache_stats_t));

    // For each trace in the file
    char rw;
    std::uint64_t address;
    while (!feof(stdin)) { 
        int ret = fscanf(stdin, "%c %" PRIx64 "\n", &rw, &address); 
        if(ret == 2) {
            cache_access(static_cast<memop_type>(rw), address, &stats); 
        }
    }

    cache_complete(&stats);

    print_statistics(&stats);

    return 0;
}

void print_statistics(cache_stats_t* p_stats) {
    printf("Cache Statistics\n");
    printf("Accesses: %" PRIu64 "\n", p_stats->accesses);
    printf("Reads: %" PRIu64 "\n", p_stats->reads);
    printf("Writes: %" PRIu64 "\n", p_stats->writes);
    printf("Misses: %" PRIu64 "\n", p_stats->misses);
    printf("Compulsory: %" PRIu64 "\n", p_stats->compulsory);
    printf("Capacity: %" PRIu64 "\n", p_stats->capacity);
    printf("Conflict: %" PRIu64 "\n", p_stats->conflict);
    printf("Write Misses: %" PRIu64 "\n", p_stats->write_misses);
    printf("Read Requests: %" PRIu64 "\n", p_stats->read_requests);
    printf("Write Requests: %" PRIu64 "\n", p_stats->write_requests);
    printf("Requests: %" PRIu64 "\n", p_stats->requests);
    printf("Hit Time: %" PRIu64 "\n", p_stats->hit_time);
    printf("Miss Penalty: %" PRIu64 "\n", p_stats->miss_penalty);
    printf("Miss Rate: %f\n", p_stats->miss_rate);
    printf("AMAT: %f\n", p_stats->amat);
    printf("Storage Overhead: %" PRIu64 "\n", p_stats->storage_overhead);
    printf("Storage Overhead Ratio: %f\n", p_stats->storage_overhead_ratio);
}

