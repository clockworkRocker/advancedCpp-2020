#include <gtest/gtest.h>
extern "C" {
#include "NRProcess.h"
}

TEST(testNeural, createArchitecture) {
  NRLayerVector pushy;
  FILE *description_file = nullptr;
  std::string path(INPUT_PATH);
  path.append("test_description");

  ASSERT_EQ(NRLayerVector_init(&pushy), 0);
  std::cout << "[ createArchitecture ] Vector initialized" << std::endl;
  description_file = fopen(path.c_str(), "r");

  EXPECT_EQ(NR_create_architecture(description_file, INPUT_PATH, &pushy), 0);
  for (size_t i = 0; i < pushy.length; ++i)
    std::cout << "[ createArchitecture ] Layer "
              << i + 1 << ": "
              << pushy.mem[i].type << "("
              << "\"" << pushy.mem[i].name << "\""
              << ", " << pushy.mem[i].input_dim << ", " << pushy.mem[i].output_dim << ')' << std::endl;
  ASSERT_EQ(NRLayerVector_delete(&pushy), 0);
  fclose(description_file);
}