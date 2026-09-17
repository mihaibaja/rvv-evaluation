#include <stddef.h>

const char *description = "Unordered RVV FP32 to FP64 widening sum of 1024 elements";
const char *instruction = "vfwredusum.vs";
const char *implementation = "vector unordered widening";
unsigned long input_bits = 32;
unsigned long accumulator_bits = 64;
unsigned long element_count = 1024;
unsigned long operations_per_iteration = 1024;

alignas(64) static float input[1024];
static double result;

void init_mem() {
  for (size_t i = 0; i < element_count; ++i)
    input[i] = (float)((double)((int)(i % 31) - 15) / 1024.0);
}

void kernel(unsigned long iterations) {
  double sum = 0.0;
  for (unsigned long iteration = 0; iteration < iterations; ++iteration) {
    const float *ptr = input;
    unsigned long remaining = element_count;
    unsigned long vl;
    unsigned long bytes;
    sum = 0.0;
    asm volatile(
        "vsetivli zero, 1, e64, m1, ta, ma\n\t"
        "vfmv.s.f v1, %[sum]\n\t"
        "1:\n\t"
        "vsetvli %[vl], %[remaining], e32, m1, ta, ma\n\t"
        "vle32.v v2, (%[ptr])\n\t"
        "vfwredusum.vs v1, v2, v1\n\t"
        "slli %[bytes], %[vl], 2\n\t"
        "add %[ptr], %[ptr], %[bytes]\n\t"
        "sub %[remaining], %[remaining], %[vl]\n\t"
        "bnez %[remaining], 1b\n\t"
        "vsetivli zero, 1, e64, m1, ta, ma\n\t"
        "vfmv.f.s %[sum], v1\n\t"
        : [sum] "+f"(sum), [ptr] "+r"(ptr), [remaining] "+r"(remaining),
          [vl] "=&r"(vl), [bytes] "=&r"(bytes)
        :
        : "v1", "v2", "memory");
  }
  result = sum;
}

double get_result() { return result; }
