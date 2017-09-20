#ifndef CACHESIM_HPP
#define CACHESIM_HPP

#include <cstdint>

struct cache_stats_t {
    std::uint64_t accesses;
    std::uint64_t reads;
    std::uint64_t writes;
    std::uint64_t misses;
    std::uint64_t compulsory;
    std::uint64_t capacity;
    std::uint64_t conflict;
    std::uint64_t write_misses;
    std::uint64_t read_requests;
    std::uint64_t write_requests;
    std::uint64_t requests;
    std::uint64_t hit_time;
    std::uint64_t miss_penalty;
    double        miss_rate;
    double        amat;
    std::uint64_t storage_overhead;
    double   storage_overhead_ratio;
};

enum memop_type {
    MEMOP_READ      = 'r',
    MEMOP_WRITE     = 'w',
};

void cache_setup(int total_size, int block_size, int associativity);
void cache_access(memop_type rw, std::uint64_t address, cache_stats_t* p_stats);
void cache_complete(cache_stats_t *p_stats);

#endif /* CACHESIM_HPP */
