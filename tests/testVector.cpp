#include "gtest/gtest.h"
#include <random>

extern "C" {
#include "../include/ht1.h"
}

intVector testy;
pyVector pushy;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> uid(100, 100000);

TEST(testIntVector, testAlloc) {
  int size = uid(gen);
  intVectorInit(&testy);
  for (int i = 0; i < size; i++)
    intVectorPush(&testy, uid(gen));

  EXPECT_EQ(testy.takenSpots, size);

  intVectorFree(&testy);
}

TEST(testPyVector, testAlloc) {
  int size = uid(gen);
  char smile[3] = {':', '3', '\0'};

  vectorInit(&pushy);
  for (int i = 0; i < size; i++)
    vectorPush(&pushy, smile, 2);

  EXPECT_EQ(pushy.takenSpots, size);

  vectorFree(&pushy);
}

