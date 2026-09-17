#include <stddef.h>

const char *description = "Scalar FP32 sum of 1024 elements";
const char *instruction = "fadd.s";
const char *implementation = "scalar";
unsigned long input_bits = 32;
unsigned long accumulator_bits = 32;
unsigned long element_count = 1024;
unsigned long operations_per_iteration = 1024;

alignas(64) static float input[1024];
static float result;

void init_mem() {
  for (size_t i = 0; i < element_count; ++i)
    input[i] = (float)((double)((int)(i % 31) - 15) / 1024.0);
}

void kernel(unsigned long iterations) {
  float sum = 0.0f;
  for (unsigned long iteration = 0; iteration < iterations; ++iteration) {
    const float *ptr = input;
    unsigned long remaining = element_count;
    sum = 0.0f;
    asm volatile(
        "1:\n\t"
        "flw ft1, 0(%[ptr])\n\t"
        "fadd.s %[sum], %[sum], ft1\n\t"
        "addi %[ptr], %[ptr], 4\n\t"
        "addi %[remaining], %[remaining], -1\n\t"
        "bnez %[remaining], 1b\n\t"
        : [sum] "+f"(sum), [ptr] "+r"(ptr), [remaining] "+r"(remaining)
        :
        : "ft1", "memory");
  }
  result = sum;
}

double get_result() { return (double)result; }
