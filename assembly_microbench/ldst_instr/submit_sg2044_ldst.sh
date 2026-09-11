#!/usr/bin/env bash
#SBATCH --job-name=rvv-eval-ldst
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=64
#SBATCH --partition=peak
#SBATCH --time=00:10:00
#SBATCH --exclusive
#SBATCH --output=%x_%j.out
#SBATCH --error=%x_%j.out

set -euo pipefail

source /etc/profile.d/modules.sh
module use /work/home/rshi/software/modules
module load openmpi/5.0.5-clang18.1.5
module load pmix/5.0.2-clang18.1.5

mkdir -p build/clang21
for bits in 16 32 64; do
  /usr/bin/clang++ -static -nostdlib++ -march=rv64gcv_zfh_zvfh \
    -o "build/clang21/vle_vv_${bits}_m1_tu_mu_1010.x" \
    "kernels/ld/vle_vv_${bits}_m1_tu_mu_1010.cpp" \
    driver_sg2044.cpp
done

BUILD_DIRS=build/clang21 \
OPS_LIST=vle \
DATABITS_LIST="16 32 64" \
MASK_CODES=1010 \
  ./run/run_vector_mask.sh
