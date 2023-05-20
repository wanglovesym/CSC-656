#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <vector>
#include <string.h>
#include <cstdlib>

void 
setup(int64_t N, uint64_t A[])
{
   std::srand(std::time(nullptr));
   printf(" inside sum_indirect problem_setup, N=%lld \n", N);
   for (int i = 0; i < N; i++) {
//      A[i] = lrand48() % N;
		A[i] = std::rand() % N; 
//		std::default_random_engine generator;
//		std::uniform_int_distribution<uint64_t> distribution(0, N-1);
//		for (int i = 0; i < N; ++i) {
//		    A[i] = distribution(generator);
//		}
   }
	
}

int64_t
sum(int64_t N, uint64_t A[])
{
   printf(" inside sum_indirect perform_sum, N=%lld \n", N);

   int64_t result = 0;
   int64_t index = A[0]; 
   for(int i = 0; i < N; i++) {
      result += A[i];
      index = A[index];
   }
   return result;
}

