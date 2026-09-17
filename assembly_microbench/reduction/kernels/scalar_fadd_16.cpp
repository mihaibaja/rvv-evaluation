#include <stddef.h>

const char *description = "Scalar FP16 sum of 1024 elements";
const char *instruction = "fadd.h";
const char *implementation = "scalar";
unsigned long input_bits = 16;
unsigned long accumulator_bits = 16;
unsigned long element_count = 1024;
unsigned long operations_per_iteration = 1024;

alignas(64) static _Float16 input[1024];
static _Float16 result;

void init_mem() {
  for (size_t i = 0; i < element_count; ++i)
    input[i] = (_Float16)((double)((int)(i % 31) - 15) / 1024.0);
}

void kernel(unsigned long iterations) {
  _Float16 sum = 0.0;
  for (unsigned long iteration = 0; iteration < iterations; ++iteration) {
    const _Float16 *ptr = input;
    unsigned long remaining = element_count;
    sum = 0.0;
    asm volatile(
        "1:\n\t"
        "flh ft1, 0(%[ptr])\n\t"
        "fadd.h %[sum], %[sum], ft1\n\t"
        "addi %[ptr], %[ptr], 2\n\t"
        "addi %[remaining], %[remaining], -1\n\t"
        "bnez %[remaining], 1b\n\t"
        : [sum] "+f"(sum), [ptr] "+r"(ptr), [remaining] "+r"(remaining)
        :
        : "ft1", "memory");
  }
  result = sum;
}

double get_result() { return (double)result; }
