# Georgia Institute of Techology
# School of Computer Science
# CS 4290/6290: Summer, 2017
# Project 3: Cache Simulator
# Due: 07/21/2017 11:55PM

Rules
-----
This is the third project for the course. As usual, here are the rules:

1. Sharing of code between students is viewed as cheating and will receive appropriate action in accordance with
   University policy. 
2. It is acceptable for you to compare your results, and only your results, with other students to help debug your
   program. It is not acceptable to collaborate either on the code development or on the final experiments. 
3. You should do all your work in the C or C++ programming language, and should be written according to the C99 or C++11
   standards, using only the standard libraries.
4. Unfortunately experience has shown that there is a very high chance that there are errors in this project
   description.  The online version will be updated as errors are discovered.  It is your responsibility to check the
   website often and download new versions of this project description as they become available. 
5. A Makefile with the frontend will be given to you; you will only need to fill in the empty functions and any
   additional subroutines you will be using. You will also need to fill in the statistics structure that will be used to
   output the results.

Project Description
--------------------
In this project, we will investigate how the performance of caches are affected by their configuration. To do so, we
will design a parameterized cache simulator, and figure out what the right performance per trade-off is.

The simulated cache is a virtually indexed, virtually tagged cache, with 64-bit virtual addresses. Blocks are replaced
in least-recently-used (LRU) order. The write policy is write-back-write-allocate. The write-back policy buffers the
writes inside the cache line, and only sends updates downstream when the back block is invalidated. Therefore to
maintain this information, you need 1 additional dirty bit for each block (in addition to the valid bit). The processor
does a read or write to an address, which are fed from a trace file.  Assume that all memory requests are for a single
byte.

For each cache block, there are two types of storage required. First, the actual data storage, which is whatever you
configured the block size to be. Second, there is metadata (aka storage overhead), which is any storage that's not the
actual data: the tag bits, valid bits, and dirty bits. For example, with 16-byte blocks and 12-bits of tag, one valid
bit and one dirty bit, the data storage is 16-bytes, whereas the metadata is 14 bits. 

The cache can be configured using the following parameters:

1. Total size in bytes
2. Block size in bytes
3. Associativity

Each numerical parameter should be in powers of 2. The total size and block size refers to the actual data storage size
of the cache.

Because the configuration reflects how the cache will perform, we will have it following times change accordingly.

1. The hit time will be ``0.2*(associativity)`` cycles
2. The miss penalty will consume ``0.2*(associativity) + 50 + 0.25*(block_size)`` cycles

All hit times and miss penalties need to be in integer, and should be round up to the nearest one.

Specification of simulator
---------------------------
Explanation of Functions you need to fill in:

* ``void cache_setup(int total_size, int block_size, int associativity)``

Initialize the cache with the given parameters. Each numerical parameter should be in powers of 2.

* ``void cache_access(memop_type rw, std::uint64_t address)``

Do a single access to the cache. Assume that each access is for a single byte.

* ``void cache_complete()``

Finalize any statistics and terminate the cache.

Statistics (output)
--------------------
The simulator outputs the following statistics after completion of the run:

1. Number of accesses to the cache
2. Number of reads
3. Number of writes
4. Number of misses
5. Number of compulsory misses
6. Number of capacity misses
7. Number of conflict misses
8. Number of write misses
9. Number of read requests to memory
10. Number of write requests to memory
11. Number of requests (read and write) to memory
12. Hit time
13. Miss penalty
14. Miss rate
15. Average memory access time (AMAT)
16. Storage overhead
17. Storage overhead ratio
18. Total storage (both data and overhead)

The categorization of misses can be a bit tricky. Remember that all cache blocks that you see for the first time ever
are compulsory misses, whereas capacity misses are misses that you experience even if you have a fully associative
cache. Conflict misses are misses due to caches having a limited size and associativity, and are those that are not the
first two.

Validation
----------
Several test traces will be provided along with the correct output. You must run your simulator and debug it until it
matches 100% all the statistics in the validation outputs posted on the website. It is highly recommended that you first
try to work through each trace for each predictor type by hand.

Experiments
-----------
For each benchmark in the traces directory, design a cache subject to the following goals: 

1. You have a total budget of ``4KB (4*1024)`` of total storage, **INCLUDING** all overhead storage.
2. The cache that has the lowest possible average memory access time (AMAT).

What to hand in via T-Square:
-----------------------------
1. The commented source code for the simulator program itself (you only need to provide cachesim.hpp, cachesim.cpp).
2. A document with the design results of the experiments for each trace file, with a persuasive argument of the choices
   that were made. (An argument may be as simple as an explanation of the search procedure used to find the designs and
   a statement about why the procedure is complete.) This argument should include output from runs of your program.
   (There are multiple answers for each trace file, so I will know which students have "collaborated" inappropriately!) 
3. Late submissions will be deducted 25%p per day

Grading
-------
0%    You do not hand in anything
+50%    Your simulator doesn't run, does not work, but you hand in significant commented code 
+35%    Your simulator matches the validation outputs
+15%    You ran all experiments and found the best-performing schedulers
