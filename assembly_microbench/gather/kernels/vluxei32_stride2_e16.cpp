#include <riscv_vector.h>
#include <stddef.h>
#include <stdint.h>

const char *description = "Peak 16-bit vluxei32.v stride-2 throughput";
const char *instruction = "vluxei32.v";
const char *index_style = "stride2 (even elements)";
unsigned long block_inst = 30;
unsigned long block_ops = 0;
unsigned long block_useful_ops = 0;
unsigned long unroll = 4;
unsigned long lanes = 0;
unsigned long element_bits = 16;

alignas(64) static uint8_t mem[8192];
alignas(64) static uint32_t indexes[1024];

void init_mem() {
  lanes = 8 * __riscv_vlenb() / element_bits;
  block_ops = block_inst * lanes;
  block_useful_ops = block_ops;
  for (size_t i = 0; i < 8192; ++i)
    mem[i] = (uint8_t)i;
  for (size_t i = 0; i < lanes; ++i)
    indexes[i] = (uint32_t)(i * 4);
}

void kernel(unsigned long iterations) {
  asm volatile("vsetvli zero, %0, e16, m1, ta, ma" : : "r"(lanes));
  asm volatile("vle32.v v30, (%0)" : : "r"(indexes) : "memory");

  for (unsigned long iteration = 0; iteration < iterations; ++iteration) {
    asm volatile(
        ".rept 4\n\t"
        "vluxei32.v v0, (%[mem]), v30\n\t"
        "vluxei32.v v1, (%[mem]), v30\n\t"
        "vluxei32.v v2, (%[mem]), v30\n\t"
        "vluxei32.v v3, (%[mem]), v30\n\t"
        "vluxei32.v v4, (%[mem]), v30\n\t"
        "vluxei32.v v5, (%[mem]), v30\n\t"
        "vluxei32.v v6, (%[mem]), v30\n\t"
        "vluxei32.v v7, (%[mem]), v30\n\t"
        "vluxei32.v v8, (%[mem]), v30\n\t"
        "vluxei32.v v9, (%[mem]), v30\n\t"
        "vluxei32.v v10, (%[mem]), v30\n\t"
        "vluxei32.v v11, (%[mem]), v30\n\t"
        "vluxei32.v v12, (%[mem]), v30\n\t"
        "vluxei32.v v13, (%[mem]), v30\n\t"
        "vluxei32.v v14, (%[mem]), v30\n\t"
        "vluxei32.v v15, (%[mem]), v30\n\t"
        "vluxei32.v v16, (%[mem]), v30\n\t"
        "vluxei32.v v17, (%[mem]), v30\n\t"
        "vluxei32.v v18, (%[mem]), v30\n\t"
        "vluxei32.v v19, (%[mem]), v30\n\t"
        "vluxei32.v v20, (%[mem]), v30\n\t"
        "vluxei32.v v21, (%[mem]), v30\n\t"
        "vluxei32.v v22, (%[mem]), v30\n\t"
        "vluxei32.v v23, (%[mem]), v30\n\t"
        "vluxei32.v v24, (%[mem]), v30\n\t"
        "vluxei32.v v25, (%[mem]), v30\n\t"
        "vluxei32.v v26, (%[mem]), v30\n\t"
        "vluxei32.v v27, (%[mem]), v30\n\t"
        "vluxei32.v v28, (%[mem]), v30\n\t"
        "vluxei32.v v29, (%[mem]), v30\n\t"
        ".endr\n\t"
        :
        : [mem] "r"(mem)
        : "memory");
  }
}
