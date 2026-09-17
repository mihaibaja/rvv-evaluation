#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern const char *description;
extern const char *instruction;
extern const char *index_style;
extern unsigned long block_inst;
extern unsigned long block_ops;
extern unsigned long block_useful_ops;
extern unsigned long unroll;
extern unsigned long lanes;
extern unsigned long element_bits;
extern void init_mem();
extern void kernel(unsigned long iterations);

int main(int argc, char **argv) {
  unsigned long iterations = 100000000ul;
  if (argc > 1)
    iterations = strtoul(argv[1], nullptr, 10);

  init_mem();
  kernel(1000);

  const unsigned long instructions_per_iteration = unroll * block_inst;
  const unsigned long elements_per_iteration = unroll * block_ops;
  const unsigned long useful_elements_per_iteration =
      unroll * block_useful_ops;
  const unsigned long total_instructions =
      iterations * instructions_per_iteration;
  const unsigned long total_elements = iterations * elements_per_iteration;
  const unsigned long total_useful_elements =
      iterations * useful_elements_per_iteration;

  timespec start{};
  timespec end{};
  clock_gettime(CLOCK_MONOTONIC, &start);
  kernel(iterations);
  clock_gettime(CLOCK_MONOTONIC, &end);

  const double seconds = (double)(end.tv_sec - start.tv_sec) +
                         (double)(end.tv_nsec - start.tv_nsec) / 1.0e9;

  printf("%s;\n", description);
  printf("Instruction;%s\n", instruction);
  printf("Index style;%s\n", index_style);
  printf("Element bits;%lu\n", element_bits);
  printf("VL;%lu\n", lanes);
  printf("Iterations;%lu\n", iterations);
  printf("Total Inst;%lu\n", total_instructions);
  printf("Total Elements;%lu\n", total_elements);
  printf("Inst/Iter;%lu\n", instructions_per_iteration);
  printf("Elements/Iter;%lu\n", elements_per_iteration);
  printf("Useful Elements/Iter;%lu\n", useful_elements_per_iteration);
  printf("Seconds;%.9f\n", seconds);
  printf("GInst/sec;%.9f\n", (double)total_instructions / seconds / 1.0e9);
  printf("GElements/sec;%.9f\n", (double)total_elements / seconds / 1.0e9);
  printf("Useful GElements/sec;%.9f\n",
         (double)total_useful_elements / seconds / 1.0e9);
  return 0;
}
