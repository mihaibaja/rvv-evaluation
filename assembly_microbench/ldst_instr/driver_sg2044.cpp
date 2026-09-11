// Minimal C-library-only driver for native static builds on SG2044.
#include <stdio.h>
#include <time.h>

extern const char* description;
extern unsigned long block_inst;
extern unsigned long block_ops;
extern unsigned long unroll;
extern void init_mem();
extern void kernel(unsigned long iters);

int main() {
  const unsigned long iterations = 100000000UL;
  const unsigned long instructions_per_iteration = unroll * block_inst;
  const unsigned long operations_per_iteration = unroll * block_ops;
  const unsigned long total_instructions =
      iterations * instructions_per_iteration;
  const unsigned long total_operations =
      iterations * operations_per_iteration;
  timespec start{};
  timespec end{};

  init_mem();
  clock_gettime(CLOCK_MONOTONIC, &start);
  kernel(iterations);
  clock_gettime(CLOCK_MONOTONIC, &end);

  const double seconds = static_cast<double>(end.tv_sec - start.tv_sec) +
                         static_cast<double>(end.tv_nsec - start.tv_nsec) /
                             1000000000.0;
  const double throughput = static_cast<double>(total_operations) /
                            seconds / 1000000000.0;

  printf("%s;\n", description);
  printf("Iterations;%lu\n", iterations);
  printf("Total Inst;%lu\n", total_instructions);
  printf("Total Ops;%lu\n", total_operations);
  printf("Inst/Iter;%lu\n", instructions_per_iteration);
  printf("Ops/Iter;%lu\n", operations_per_iteration);
  printf("Seconds;%.9f\n", seconds);
  printf("GOps/sec;%.9f\n", throughput);
  return 0;
}
