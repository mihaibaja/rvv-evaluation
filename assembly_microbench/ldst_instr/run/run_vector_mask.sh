#!/bin/bash
set -euo pipefail

BUILD_DIRS=${BUILD_DIRS:-"build/clang21 build/gcc15"}
OPS_LIST=${OPS_LIST:-"vle vse"}
DATABITS_LIST=${DATABITS_LIST:-"8 16 32 64"}
MASK_CODES=${MASK_CODES:-""}

for BUILD_DIR in ${BUILD_DIRS}; do
  echo "== ${BUILD_DIR} =="
  for OPS in ${OPS_LIST}; do
    for RES in v; do
      for lmul in m1; do
        for databits in ${DATABITS_LIST}; do
          for tail in tu; do
            for mask in mu; do
              if [ -n "${MASK_CODES}" ]; then
                maskcodes="${MASK_CODES}"
              elif [ "${databits}" = 8 ]; then
                maskcodes="1010 1 2 4 8 16 32"
              else
                maskcodes="1010"
              fi
              for maskcode in ${maskcodes}; do
                exe="${OPS}_v${RES}_${databits}_${lmul}_${tail}_${mask}_${maskcode}"
                echo "${exe}"
                "${BUILD_DIR}/${exe}.x"
              done
            done
          done
        done
      done
    done
  done
done
