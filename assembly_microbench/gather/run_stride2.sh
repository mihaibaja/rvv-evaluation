#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR=${BUILD_DIR:-build}
ITERATIONS=${ITERATIONS:-1000000}

for bits in 8 16 32 64; do
  for name in "vluxei32_stride2_e${bits}" "vlseg2e${bits}"; do
    echo "== ${BUILD_DIR}/${name}.x =="
    "${BUILD_DIR}/${name}.x" "${ITERATIONS}"
  done
done
