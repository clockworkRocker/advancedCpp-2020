#!/bin/bash

if [ ! -d "build" ]
then  mkdir -p "build"
fi
cd build

echo "\n[Log] Running cppcheck"
cppcheck --enable=all --suppress=missingInclude ../src/* ../tests/*.cpp

echo "\n[Log] Building project"
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTING=YES -DENABLE_COVERAGE=YES ..
if test $? -ne 0
then
  echo "[Log] ERROR! CMake failed"
  exit 1;
fi

make
if test $? -ne 0
then
  echo "[Log] ERROR! Make failed"
  exit 1;
fi
echo "\n[Log] Running tests"
valgrind --leak-check=full ./test_FakeNeuralNetwork