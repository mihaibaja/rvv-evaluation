
#include <riscv_vector.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdio.h>

const char * description = "4( 30(rv64gcv_VSE64.V_64b) )";
unsigned long block_inst = 30;
unsigned long lanes = ((int)8*__riscv_vlenb()*1/64);
unsigned long block_ops = (30*(1*lanes))/2;
unsigned long unroll = 4;
static int64_t *mem = nullptr;
static int avl = 1;
void init_mem()
{
    
    // Build the alternating predicate at the full e8 VLMAX.  On SG2044,
    // using the smaller target-SEW lane count here can trap.
    unsigned long mask_lanes = ((int)8*__riscv_vlenb()*1/8);
    asm volatile ("vsetvli %0, %1, e8, m1, tu, mu": "=r"(avl): "r"(mask_lanes));
    asm volatile ("vid.v      v1\n\t"
                  "vand.vi    v1, v1, 1 \n\t"
                  "vmseq.vi   v0, v1, 0 \n\t"
                  :
                  :
                  : "v0", "v1");
    // A library call here may clobber vector state.  Change to the target SEW
    // immediately after v0 has been populated.
    asm volatile ("vsetvli %0, %1, e64, m1, tu, mu": "=r"(avl): "r"(lanes));
        
    

    mem = (int64_t *)malloc(avl * sizeof(int64_t));
    srand(time(NULL));
    for (size_t i = 0; i < avl; i++){
	    mem[i] = ( int64_t )((int)rand());
    printf("init value%ld=%f\t",i, (double)mem[i]);
   }
   printf("\n");
    
}

void kernel(unsigned long iters)
{
    
    // Build the alternating predicate at the full e8 VLMAX.  On SG2044,
    // using the smaller target-SEW lane count here can trap.
    unsigned long mask_lanes = ((int)8*__riscv_vlenb()*1/8);
    asm volatile ("vsetvli %0, %1, e8, m1, tu, mu": "=r"(avl): "r"(mask_lanes));
    asm volatile ("vid.v      v1\n\t"
                  "vand.vi    v1, v1, 1 \n\t"
                  "vmseq.vi   v0, v1, 0 \n\t"
                  :
                  :
                  : "v0", "v1");
    // A library call here may clobber vector state.  Change to the target SEW
    // immediately after v0 has been populated.
    asm volatile ("vsetvli %0, %1, e64, m1, tu, mu": "=r"(avl): "r"(lanes));
        

    for (unsigned long i=0; i<iters; ++i) {
        asm volatile (
        "vse64.v v2, (%0), v0.t  \n\t"
        "vse64.v v3, (%0), v0.t  \n\t"
        "vse64.v v4, (%0), v0.t  \n\t"
        "vse64.v v5, (%0), v0.t  \n\t"
        "vse64.v v6, (%0), v0.t  \n\t"
        "vse64.v v7, (%0), v0.t  \n\t"
        "vse64.v v8, (%0), v0.t  \n\t"
        "vse64.v v9, (%0), v0.t  \n\t"
        "vse64.v v10, (%0), v0.t  \n\t"
        "vse64.v v11, (%0), v0.t  \n\t"
        "vse64.v v12, (%0), v0.t  \n\t"
        "vse64.v v13, (%0), v0.t  \n\t"
        "vse64.v v14, (%0), v0.t  \n\t"
        "vse64.v v15, (%0), v0.t  \n\t"
        "vse64.v v16, (%0), v0.t  \n\t"
        "vse64.v v17, (%0), v0.t  \n\t"
        "vse64.v v18, (%0), v0.t  \n\t"
        "vse64.v v19, (%0), v0.t  \n\t"
        "vse64.v v20, (%0), v0.t  \n\t"
        "vse64.v v21, (%0), v0.t  \n\t"
        "vse64.v v22, (%0), v0.t  \n\t"
        "vse64.v v23, (%0), v0.t  \n\t"
        "vse64.v v24, (%0), v0.t  \n\t"
        "vse64.v v25, (%0), v0.t  \n\t"
        "vse64.v v26, (%0), v0.t  \n\t"
        "vse64.v v27, (%0), v0.t  \n\t"
        "vse64.v v28, (%0), v0.t  \n\t"
        "vse64.v v29, (%0), v0.t  \n\t"
        "vse64.v v30, (%0), v0.t  \n\t"
        "vse64.v v2, (%0), v0.t  \n\t"
        "vse64.v v2, (%0), v0.t  \n\t"
        "vse64.v v3, (%0), v0.t  \n\t"
        "vse64.v v4, (%0), v0.t  \n\t"
        "vse64.v v5, (%0), v0.t  \n\t"
        "vse64.v v6, (%0), v0.t  \n\t"
        "vse64.v v7, (%0), v0.t  \n\t"
        "vse64.v v8, (%0), v0.t  \n\t"
        "vse64.v v9, (%0), v0.t  \n\t"
        "vse64.v v10, (%0), v0.t  \n\t"
        "vse64.v v11, (%0), v0.t  \n\t"
        "vse64.v v12, (%0), v0.t  \n\t"
        "vse64.v v13, (%0), v0.t  \n\t"
        "vse64.v v14, (%0), v0.t  \n\t"
        "vse64.v v15, (%0), v0.t  \n\t"
        "vse64.v v16, (%0), v0.t  \n\t"
        "vse64.v v17, (%0), v0.t  \n\t"
        "vse64.v v18, (%0), v0.t  \n\t"
        "vse64.v v19, (%0), v0.t  \n\t"
        "vse64.v v20, (%0), v0.t  \n\t"
        "vse64.v v21, (%0), v0.t  \n\t"
        "vse64.v v22, (%0), v0.t  \n\t"
        "vse64.v v23, (%0), v0.t  \n\t"
        "vse64.v v24, (%0), v0.t  \n\t"
        "vse64.v v25, (%0), v0.t  \n\t"
        "vse64.v v26, (%0), v0.t  \n\t"
        "vse64.v v27, (%0), v0.t  \n\t"
        "vse64.v v28, (%0), v0.t  \n\t"
        "vse64.v v29, (%0), v0.t  \n\t"
        "vse64.v v30, (%0), v0.t  \n\t"
        "vse64.v v2, (%0), v0.t  \n\t"
        "vse64.v v2, (%0), v0.t  \n\t"
        "vse64.v v3, (%0), v0.t  \n\t"
        "vse64.v v4, (%0), v0.t  \n\t"
        "vse64.v v5, (%0), v0.t  \n\t"
        "vse64.v v6, (%0), v0.t  \n\t"
        "vse64.v v7, (%0), v0.t  \n\t"
        "vse64.v v8, (%0), v0.t  \n\t"
        "vse64.v v9, (%0), v0.t  \n\t"
        "vse64.v v10, (%0), v0.t  \n\t"
        "vse64.v v11, (%0), v0.t  \n\t"
        "vse64.v v12, (%0), v0.t  \n\t"
        "vse64.v v13, (%0), v0.t  \n\t"
        "vse64.v v14, (%0), v0.t  \n\t"
        "vse64.v v15, (%0), v0.t  \n\t"
        "vse64.v v16, (%0), v0.t  \n\t"
        "vse64.v v17, (%0), v0.t  \n\t"
        "vse64.v v18, (%0), v0.t  \n\t"
        "vse64.v v19, (%0), v0.t  \n\t"
        "vse64.v v20, (%0), v0.t  \n\t"
        "vse64.v v21, (%0), v0.t  \n\t"
        "vse64.v v22, (%0), v0.t  \n\t"
        "vse64.v v23, (%0), v0.t  \n\t"
        "vse64.v v24, (%0), v0.t  \n\t"
        "vse64.v v25, (%0), v0.t  \n\t"
        "vse64.v v26, (%0), v0.t  \n\t"
        "vse64.v v27, (%0), v0.t  \n\t"
        "vse64.v v28, (%0), v0.t  \n\t"
        "vse64.v v29, (%0), v0.t  \n\t"
        "vse64.v v30, (%0), v0.t  \n\t"
        "vse64.v v2, (%0), v0.t  \n\t"
        "vse64.v v2, (%0), v0.t  \n\t"
        "vse64.v v3, (%0), v0.t  \n\t"
        "vse64.v v4, (%0), v0.t  \n\t"
        "vse64.v v5, (%0), v0.t  \n\t"
        "vse64.v v6, (%0), v0.t  \n\t"
        "vse64.v v7, (%0), v0.t  \n\t"
        "vse64.v v8, (%0), v0.t  \n\t"
        "vse64.v v9, (%0), v0.t  \n\t"
        "vse64.v v10, (%0), v0.t  \n\t"
        "vse64.v v11, (%0), v0.t  \n\t"
        "vse64.v v12, (%0), v0.t  \n\t"
        "vse64.v v13, (%0), v0.t  \n\t"
        "vse64.v v14, (%0), v0.t  \n\t"
        "vse64.v v15, (%0), v0.t  \n\t"
        "vse64.v v16, (%0), v0.t  \n\t"
        "vse64.v v17, (%0), v0.t  \n\t"
        "vse64.v v18, (%0), v0.t  \n\t"
        "vse64.v v19, (%0), v0.t  \n\t"
        "vse64.v v20, (%0), v0.t  \n\t"
        "vse64.v v21, (%0), v0.t  \n\t"
        "vse64.v v22, (%0), v0.t  \n\t"
        "vse64.v v23, (%0), v0.t  \n\t"
        "vse64.v v24, (%0), v0.t  \n\t"
        "vse64.v v25, (%0), v0.t  \n\t"
        "vse64.v v26, (%0), v0.t  \n\t"
        "vse64.v v27, (%0), v0.t  \n\t"
        "vse64.v v28, (%0), v0.t  \n\t"
        "vse64.v v29, (%0), v0.t  \n\t"
        "vse64.v v30, (%0), v0.t  \n\t"
        "vse64.v v2, (%0), v0.t  \n\t"
        : /* no input */
        : "r"(mem)
        : "memory"
        );
    }

        int64_t buf[32];
        asm volatile(
            "vse64.v v2, (%0)\n\t"
            :
            : "r"(buf)
            : "t0", "t1", "memory"
        );

        printf("Check if the v0 register has been written to\n");

        for (size_t i = 0; i < avl; i++)
            printf("element%zu=%-10f\t", i, (double)buf[i]);
        printf("\n");
                         
free(mem);
}

