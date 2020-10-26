#include <gtest/gtest.h>

extern "C" {
#include "../include/ht1.h"
#include "../include/pyVector.h"
}

const char *examplePath = NULL;

TEST(testGeneral, testWithExample) {
  FILE *file;
  pyVector functions;

  file = fopen(examplePath, "r");

  ASSERT_NE(file, nullptr);

  vectorInit(&functions);
  collectPyFunctionsFromFile(file, &functions);

  EXPECT_EQ(functions.takenSpots, 4);
  EXPECT_EQ(functions.array[0].lines.takenSpots, 2);

  vectorFree(&functions);
  fclose(file);
}

TEST(testGeneral, testCheckIdentifier) {
  const char *strArray[7] = {"", "1", "25=", "not_an_identifier", "bl3u", "&&&", "glue"};
  int results[7] = {false, false, false, true, false, false, true};
  for (int i = 0; i < 7; i++) {
    EXPECT_EQ(checkForIdentifier(strArray[i], strlen(strArray[i])), results[i])
              << "checkForIdentifier failed on " << strArray[i];
  }
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  if (argc < 2) {
    examplePath = "Examples/example.py";
  } else
    examplePath = argv[1];

  return RUN_ALL_TESTS();
}
