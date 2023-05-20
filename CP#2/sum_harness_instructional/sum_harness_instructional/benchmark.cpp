//
// (C) 2022, E. Wes Bethel
// benchmark-* harness for running different versions of the sum study
//    over different problem sizes
//
// usage: no command line arguments
// set problem sizes, block sizes in the code below

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <cstdlib>
#include <cstring>
//#include <sys/sysinfo.h>
#include <windows.h>
#include "sum_indirect.cpp"

extern void setup(int64_t N, uint64_t A[]);
extern int64_t sum(int64_t N, uint64_t A[]);

long long get_physical_memory_size() {
//    struct sysinfo memInfo;
//    sysinfo(&memInfo);
//    long long totalPhysMem = memInfo.totalram;
//    totalPhysMem *= memInfo.mem_unit;
//    return totalPhysMem;
 	MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
	return status.ullTotalPhys;
}

double calculate_mflops(int64_t num_ops, double elapsed_time) {
    const double mflops = num_ops / (elapsed_time * 1e6);
    return mflops;
}

double measure_memory_bandwidth(int64_t num_bytes, double elapsed_time) {
    const double physical_memory_size = get_physical_memory_size();
    const double num_bytes_per_sec = num_bytes  / (elapsed_time * 0.001);
    const double memory_bandwidth_percentage = (num_bytes_per_sec / physical_memory_size) * 100;
    return memory_bandwidth_percentage;
}

double measure_memory_latency(int64_t num_bytes, double elapsed_time) {
    const double num_accesses = num_bytes / 8;
    const double avg_latency = elapsed_time * 1e6 / num_accesses;
    return avg_latency;
}
/* The benchmarking program */
int main(int argc, char** argv) 
{
   std::cout << std::fixed << std::setprecision(2);

   #define MAX_PROBLEM_SIZE 1 << 28  //  256M
   std::vector<int64_t> problem_sizes{ MAX_PROBLEM_SIZE >> 5, MAX_PROBLEM_SIZE >> 4, MAX_PROBLEM_SIZE >> 3, MAX_PROBLEM_SIZE >> 2, MAX_PROBLEM_SIZE >> 1, MAX_PROBLEM_SIZE};

//   std::vector<int64_t> problem_sizes{ MAX_PROBLEM_SIZE >> 5 };
   
   std::vector<uint64_t> A(MAX_PROBLEM_SIZE);

   int64_t t;
   int n_problems = problem_sizes.size();

   /* For each test size */
   for (int64_t n : problem_sizes) 
   {
      printf("Working on problem size N=%d \n", n);

      // invoke user code to set up the problem
      setup(n, &A[0]);

      // insert your timer code here
      // start the timer
      auto start_time = std::chrono::high_resolution_clock::now();

      // invoke method to perform the sum
      t = sum(n, &A[0]);

	  // stop the timer
      auto end_time = std::chrono::high_resolution_clock::now();

      // calculate the elapsed time
      auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();

      // print out elapsed time for this problem size
      printf("Elapsed time: %lld ms\n", elapsed_time);


      // Measure memory bandwidth utilization
      double bandwidth = measure_memory_bandwidth(n * sizeof(uint64_t), elapsed_time);

      // Measure memory latency
      double latency = measure_memory_latency(n * sizeof(uint64_t), elapsed_time);

      // Calculate MFLOP/s
      double mflops = calculate_mflops(n, elapsed_time);

      // Print the results
      std::cout << "N = " << n << ", Result = " << t << std::endl;
      std::cout << "Memory Bandwidth Utilization: " << bandwidth << "%" << std::endl;
      std::cout << "Memory Latency: " << latency << " ns" << std::endl;
      std::cout << "MFLOP/s: " << mflops << std::endl;
      // insert your end timer code here, and print out elapsed time for this problem size

      printf(" Sum result = %lld \n",t);

   } // end loop over problem sizes
}


// EOF
