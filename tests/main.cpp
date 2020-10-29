#include <gtest/gtest.h>
#include <ctime>

extern "C" {
#include "NRProcess.h"
}

void resource_free(FILE *input, FILE *description, FILE *output, Matrix *matrix, NRLayerVector *layer_vector) {
  fclose(input);
  fclose(description);
  fclose(output);

  Matrix_delete(matrix);
  free(matrix);

  NRLayerVector_delete(layer_vector);
}

TEST(stressTests, testLinear){
  FILE *input_file;
  FILE *output_file;
  FILE *description_file;
  NRLayerVector description;
  Matrix *matrix = nullptr;
  size_t n_objects, n_qualities;
  clock_t timer;

  std::string description_path(STRESS_TEST_PATH);
  std::string description_file_path(STRESS_TEST_PATH);
  std::string inputMatrix_path(STRESS_TEST_PATH);
  std::string outputMatrix_path(STRESS_TEST_PATH);

  description_path.append("input/");
  description_file_path.append("input/description");
  inputMatrix_path.append("input/inputMatrix");
  outputMatrix_path.append("output/output_linear.txt");

  input_file = fopen(inputMatrix_path.c_str(), "r");
  output_file = fopen(outputMatrix_path.c_str(), "w");
  description_file = fopen(description_file_path.c_str(), "r");

  ASSERT_NE(input_file, nullptr);
  ASSERT_NE(output_file, nullptr);
  ASSERT_NE(description_file, nullptr);

  NRLayerVector_init(&description);

  ASSERT_EQ(NR_create_architecture(description_file, description_path.c_str(), &description), 0);

  fscanf(input_file,"%lu, %lu", &n_objects, &n_qualities);
  ASSERT_NE(matrix = (Matrix*)malloc(sizeof(Matrix)), nullptr);
  ASSERT_EQ(Matrix_init(matrix, n_qualities, n_objects), 0);
  EXPECT_EQ(Matrix_fill_from_stream(matrix, input_file), 0);

  timer = clock();
  EXPECT_EQ(NR_process_linear(matrix, &description, output_file), 0);
  timer = clock() - timer;

  std::cout << "[stressTestLinear] Time: " << (double)timer / CLOCKS_PER_SEC << " s." << std::endl;

  resource_free(input_file, description_file, output_file, matrix, &description);
}

TEST(stressTests, testParallel){
  FILE *input_file;
  FILE *output_file;
  FILE *description_file;
  NRLayerVector description;
  Matrix *matrix = nullptr;
  size_t n_objects, n_qualities;
  clock_t timer;

  std::string description_path(STRESS_TEST_PATH);
  std::string description_file_path(STRESS_TEST_PATH);
  std::string inputMatrix_path(STRESS_TEST_PATH);
  std::string outputMatrix_path(STRESS_TEST_PATH);

  description_path.append("input/");
  description_file_path.append("input/description");
  inputMatrix_path.append("input/inputMatrix");
  outputMatrix_path.append("output/output_linear.txt");

  input_file = fopen(inputMatrix_path.c_str(), "r");
  output_file = fopen(outputMatrix_path.c_str(), "w");
  description_file = fopen(description_file_path.c_str(), "r");

  ASSERT_NE(input_file, nullptr);
  ASSERT_NE(output_file, nullptr);
  ASSERT_NE(description_file, nullptr);

  NRLayerVector_init(&description);

  ASSERT_EQ(NR_create_architecture(description_file, description_path.c_str(), &description), 0);

  fscanf(input_file,"%lu, %lu", &n_objects, &n_qualities);
  ASSERT_NE(matrix = (Matrix*)malloc(sizeof(Matrix)), nullptr);
  ASSERT_EQ(Matrix_init(matrix, n_qualities, n_objects), 0);
  EXPECT_EQ(Matrix_fill_from_stream(matrix, input_file), 0);

  timer = clock();
  EXPECT_EQ(NR_process_parallel(matrix, &description, output_file), 0);
  timer = clock() - timer;

  std::cout << "[stressTestParallel] Time: " << (double)timer / CLOCKS_PER_SEC << " s." << std::endl;

  resource_free(input_file, description_file, output_file, matrix, &description);
}

int main(int argc, char* argv[]){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}