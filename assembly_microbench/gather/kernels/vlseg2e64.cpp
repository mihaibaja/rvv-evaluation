#include <riscv_vector.h>
#include <stddef.h>
#include <stdint.h>

const char *description = "Peak two-field 64-bit segment-load throughput";
const char *instruction = "vlseg2e64.v";
const char *index_style = "field 0 even, field 1 odd";
unsigned long block_inst = 16;
unsigned long block_ops = 0;
unsigned long block_useful_ops = 0;
unsigned long unroll = 4;
unsigned long lanes = 0;
unsigned long element_bits = 64;

alignas(64) static uint8_t mem[8192];

void init_mem() {
  lanes = 8 * __riscv_vlenb() / element_bits;
  block_ops = block_inst * 2 * lanes;
  block_useful_ops = block_inst * lanes;
  for (size_t i = 0; i < 8192; ++i)
    mem[i] = (uint8_t)i;
}

void kernel(unsigned long iterations) {
  asm volatile("vsetvli zero, %0, e64, m1, ta, ma" : : "r"(lanes));

  for (unsigned long iteration = 0; iteration < iterations; ++iteration) {
    asm volatile(
        ".rept 4\n\t"
        "vlseg2e64.v v0, (%[mem])\n\t"
        "vlseg2e64.v v2, (%[mem])\n\t"
        "vlseg2e64.v v4, (%[mem])\n\t"
        "vlseg2e64.v v6, (%[mem])\n\t"
        "vlseg2e64.v v8, (%[mem])\n\t"
        "vlseg2e64.v v10, (%[mem])\n\t"
        "vlseg2e64.v v12, (%[mem])\n\t"
        "vlseg2e64.v v14, (%[mem])\n\t"
        "vlseg2e64.v v16, (%[mem])\n\t"
        "vlseg2e64.v v18, (%[mem])\n\t"
        "vlseg2e64.v v20, (%[mem])\n\t"
        "vlseg2e64.v v22, (%[mem])\n\t"
        "vlseg2e64.v v24, (%[mem])\n\t"
        "vlseg2e64.v v26, (%[mem])\n\t"
        "vlseg2e64.v v28, (%[mem])\n\t"
        "vlseg2e64.v v30, (%[mem])\n\t"
        ".endr\n\t"
        :
        : [mem] "r"(mem)
        : "memory");
  }
}
