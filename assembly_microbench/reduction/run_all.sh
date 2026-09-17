#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR=${BUILD_DIR:-build}
ITERATIONS=${ITERATIONS:-10000}

for executable in "${BUILD_DIR}"/*.x; do
  echo "== ${executable} =="
  "${executable}" "${ITERATIONS}"
done
