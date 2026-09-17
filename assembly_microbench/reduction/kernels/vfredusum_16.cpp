#include <stddef.h>

const char *description = "Unordered RVV FP16 sum of 1024 elements";
const char *instruction = "vfredusum.vs";
const char *implementation = "vector unordered";
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
    unsigned long vl;
    unsigned long bytes;
    sum = 0.0;
    asm volatile(
        "vsetivli zero, 1, e16, m1, ta, ma\n\t"
        "vfmv.s.f v1, %[sum]\n\t"
        "1:\n\t"
        "vsetvli %[vl], %[remaining], e16, m1, ta, ma\n\t"
        "vle16.v v2, (%[ptr])\n\t"
        "vfredusum.vs v1, v2, v1\n\t"
        "slli %[bytes], %[vl], 1\n\t"
        "add %[ptr], %[ptr], %[bytes]\n\t"
        "sub %[remaining], %[remaining], %[vl]\n\t"
        "bnez %[remaining], 1b\n\t"
        "vfmv.f.s %[sum], v1\n\t"
        : [sum] "+f"(sum), [ptr] "+r"(ptr), [remaining] "+r"(remaining),
          [vl] "=&r"(vl), [bytes] "=&r"(bytes)
        :
        : "v1", "v2", "memory");
  }
  result = sum;
}

double get_result() { return (double)result; }
