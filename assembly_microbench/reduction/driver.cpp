#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern const char *description;
extern const char *instruction;
extern const char *implementation;
extern unsigned long input_bits;
extern unsigned long accumulator_bits;
extern unsigned long element_count;
extern unsigned long operations_per_iteration;
extern void init_mem();
extern void kernel(unsigned long iterations);
extern double get_result();

int main(int argc, char **argv) {
  unsigned long iterations = 10000;
  if (argc > 1)
    iterations = strtoul(argv[1], nullptr, 10);

  init_mem();
  kernel(32); // Warm the input array and instruction path.

  timespec start{};
  timespec end{};
  clock_gettime(CLOCK_MONOTONIC, &start);
  kernel(iterations);
  clock_gettime(CLOCK_MONOTONIC, &end);

  const double seconds = (double)(end.tv_sec - start.tv_sec) +
                         (double)(end.tv_nsec - start.tv_nsec) / 1.0e9;
  const unsigned long total_operations =
      iterations * operations_per_iteration;
  const double throughput =
      (double)total_operations / seconds / 1.0e9;

  printf("%s;\n", description);
  printf("Instruction;%s\n", instruction);
  printf("Implementation;%s\n", implementation);
  printf("Input bits;%lu\n", input_bits);
  printf("Accumulator bits;%lu\n", accumulator_bits);
  printf("Elements;%lu\n", element_count);
  printf("Iterations;%lu\n", iterations);
  printf("Total Ops;%lu\n", total_operations);
  printf("Seconds;%.9f\n", seconds);
  printf("GElements/sec;%.9f\n", throughput);
  printf("Result;%.17g\n", get_result());
  return 0;
}
