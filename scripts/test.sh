#!/usr/bin/env bash

SYSTEM_NAME=$1
CC_NAME=$2

if [[ "${CC_NAME}" == "gcc"* ]]; then
  CXX_NAME="g++"
elif [[ "${CC_NAME}" == "clang"* ]]; then
  CXX_NAME="clang++"
else
  echo "Compiler is not supported: ${CC_NAME}" && exit 1
fi

mkdir -p ./build
cd ./build || exit 1
if [[ "${SYSTEM_NAME}" == "alpine"* ]]; then
  cmake -D CMAKE_C_COMPILER="${CC_NAME}" \
        -D CMAKE_CXX_COMPILER="${CXX_NAME}" \
        -D CMAKE_BUILD_TYPE=Release \
        -D XW_USE_SQLITE3=yes \
        -D CONFIGURE_TESTS=ON \
        ..
elif [[ "${SYSTEM_NAME}" == "ubuntu"* ]]; then
  cmake -D CMAKE_BUILD_TYPE=Release \
        -D XW_USE_SQLITE3=yes \
        -D CONFIGURE_TESTS=ON \
        ..
else
  echo "System is not supported: ${SYSTEM_NAME}" && exit 1
fi
make unittests-all

# Run tests using valgrind.
valgrind --leak-check=full ./tests/unittests-all
