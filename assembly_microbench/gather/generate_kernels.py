#!/usr/bin/env python3
from pathlib import Path


KERNEL_DIR = Path(__file__).resolve().parent / "kernels"


def register_loads(instruction: str, registers: range, operand: str) -> str:
    return "\n".join(
        f'        "{instruction} v{register}, {operand}\\n\\t"'
        for register in registers
    )


def stride2_gather_source(element_bits: int) -> str:
    index_register = {8: 28, 16: 30, 32: 31, 64: 31}[element_bits]
    byte_stride = 2 * element_bits // 8
    loads = register_loads(
        "vluxei32.v", range(index_register),
        f"(%[mem]), v{index_register}",
    )
    return f'''#include <riscv_vector.h>
#include <stddef.h>
#include <stdint.h>

const char *description = "Peak {element_bits}-bit vluxei32.v stride-2 throughput";
const char *instruction = "vluxei32.v";
const char *index_style = "stride2 (even elements)";
unsigned long block_inst = {index_register};
unsigned long block_ops = 0;
unsigned long block_useful_ops = 0;
unsigned long unroll = 4;
unsigned long lanes = 0;
unsigned long element_bits = {element_bits};

alignas(64) static uint8_t mem[8192];
alignas(64) static uint32_t indexes[1024];

void init_mem() {{
  lanes = 8 * __riscv_vlenb() / element_bits;
  block_ops = block_inst * lanes;
  block_useful_ops = block_ops;
  for (size_t i = 0; i < 8192; ++i)
    mem[i] = (uint8_t)i;
  for (size_t i = 0; i < lanes; ++i)
    indexes[i] = (uint32_t)(i * {byte_stride});
}}

void kernel(unsigned long iterations) {{
  asm volatile("vsetvli zero, %0, e{element_bits}, m1, ta, ma" : : "r"(lanes));
  asm volatile("vle32.v v{index_register}, (%0)" : : "r"(indexes) : "memory");

  for (unsigned long iteration = 0; iteration < iterations; ++iteration) {{
    asm volatile(
        ".rept 4\\n\\t"
{loads}
        ".endr\\n\\t"
        :
        : [mem] "r"(mem)
        : "memory");
  }}
}}
'''


def segment2_source(element_bits: int) -> str:
    loads = register_loads(
        f"vlseg2e{element_bits}.v", range(0, 32, 2), "(%[mem])"
    )
    return f'''#include <riscv_vector.h>
#include <stddef.h>
#include <stdint.h>

const char *description = "Peak two-field {element_bits}-bit segment-load throughput";
const char *instruction = "vlseg2e{element_bits}.v";
const char *index_style = "field 0 even, field 1 odd";
unsigned long block_inst = 16;
unsigned long block_ops = 0;
unsigned long block_useful_ops = 0;
unsigned long unroll = 4;
unsigned long lanes = 0;
unsigned long element_bits = {element_bits};

alignas(64) static uint8_t mem[8192];

void init_mem() {{
  lanes = 8 * __riscv_vlenb() / element_bits;
  block_ops = block_inst * 2 * lanes;
  block_useful_ops = block_inst * lanes;
  for (size_t i = 0; i < 8192; ++i)
    mem[i] = (uint8_t)i;
}}

void kernel(unsigned long iterations) {{
  asm volatile("vsetvli zero, %0, e{element_bits}, m1, ta, ma" : : "r"(lanes));

  for (unsigned long iteration = 0; iteration < iterations; ++iteration) {{
    asm volatile(
        ".rept 4\\n\\t"
{loads}
        ".endr\\n\\t"
        :
        : [mem] "r"(mem)
        : "memory");
  }}
}}
'''


def main() -> None:
    KERNEL_DIR.mkdir(exist_ok=True)
    for element_bits in (8, 16, 32, 64):
        (KERNEL_DIR / f"vluxei32_stride2_e{element_bits}.cpp").write_text(
            stride2_gather_source(element_bits)
        )
        (KERNEL_DIR / f"vlseg2e{element_bits}.cpp").write_text(
            segment2_source(element_bits)
        )


if __name__ == "__main__":
    main()
