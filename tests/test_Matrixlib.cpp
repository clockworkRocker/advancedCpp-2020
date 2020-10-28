#include <gtest/gtest.h>
#include <string>

extern "C" {
#include "matrix.h"
}

#define TEST_LEN 20
#define TEST_WID 20

Matrix m;

Vector v;

void FillMatrix(Matrix* matrix){
  for(size_t i = 0; i < matrix->width; ++i)
    for (size_t j = 0; j < matrix->length; ++j)
      matrix->cell[i][j] = (double )i+j;
}

TEST(testMatrix, testAlloc) {
  Matrix_init(&m, TEST_LEN, TEST_WID);

  for (int i = 0; i < TEST_WID; ++i)
    for (int j = 0; j < TEST_LEN; ++j)
      m.cell[i][j] = i + j;

  EXPECT_EQ(Matrix_delete(&m), 0);
}

TEST(testMatrix, testReuse) {
  EXPECT_EQ(Matrix_init(nullptr, TEST_LEN, TEST_WID), -1); // Test with nullptr
  EXPECT_EQ(Matrix_init(&m, -TEST_LEN, TEST_WID), -1);     // Test with negative length
  EXPECT_EQ(Matrix_init(&m, TEST_LEN, TEST_WID), 0);

  FillMatrix(&m);

  EXPECT_EQ(Matrix_print(&m, nullptr), -1);
  EXPECT_EQ(Matrix_print(&m, stdout), 0);

  Matrix_delete(&m);
}

TEST(testMatrix, testAdd) {
  const int expected_value = 2 * (TEST_LEN - 1 + TEST_WID - 1);

  ASSERT_EQ(Matrix_init(&m, TEST_LEN, TEST_WID), 0);
  FillMatrix(&m);

  ASSERT_EQ(Matrix_add(&m, &m), 0);
  EXPECT_EQ(m.cell[TEST_WID - 1][TEST_LEN - 1], expected_value);

  Matrix_delete(&m);
}

TEST(testMatrix, testAddVector) {
  ASSERT_EQ(Vector_init(&v, TEST_LEN), 0);
  ASSERT_EQ(Matrix_init(&m, TEST_LEN, TEST_WID), 0);

  FillMatrix(&m);

  for (int i = 0; i < TEST_LEN; ++i)
    v.mem[i] = TEST_LEN - i;

  EXPECT_EQ(Matrix_add_Vector(&m, nullptr), -1);
  ASSERT_EQ(Matrix_add_Vector(&m, &v), 0);
  ASSERT_EQ(m.cell[TEST_WID - 1][TEST_LEN - 1], TEST_LEN + TEST_LEN - 1);

  Vector_delete(&v);
  Matrix_delete(&m);
}

TEST(testMatrix, testMultiply) {
  Matrix m2;
  m2.cell = nullptr;

  Matrix result;
  result.cell = nullptr;
  ASSERT_EQ(Matrix_init(&m, TEST_LEN, TEST_WID), 0);
  ASSERT_EQ(Matrix_init(&m2, 1, TEST_WID), 0); // ;

  ASSERT_EQ(Matrix_multiply(&m, &m2, &result), 0);

  Matrix_delete(&m2);
  Matrix_delete(&result);
  Matrix_delete(&m);
}

TEST(testMatrix, testFillFromStream_1) {
  FILE *file = nullptr;
  std::string path(INPUT_PATH);
  path.append("test_MatrixFillFromStream_correct");
  EXPECT_EQ(Matrix_fill_from_stream(&m, file), -1);

  Matrix_init(&m, TEST_WID, TEST_LEN);

  file = fopen(path.c_str(), "r");
  ASSERT_NE(file, nullptr);

  EXPECT_EQ(Matrix_fill_from_stream(&m, file), 0);
  EXPECT_EQ(m.cell[TEST_WID-1][TEST_LEN-1], 38);

  fclose(file);
  Matrix_delete(&m);
}

TEST(testMatrix, testFillFromStream_2) {
  FILE *file = nullptr;
  std::string path(INPUT_PATH);
  path.append("test_MatrixFillFromStream_wrong");

  EXPECT_EQ(Matrix_fill_from_stream(&m, file), -1);

  Matrix_init(&m, TEST_WID, TEST_LEN);

  file = fopen(path.c_str(), "r");
  ASSERT_NE(file, nullptr);

  EXPECT_EQ(Matrix_fill_from_stream(&m, file), -1);

  fclose(file);
  Matrix_delete(&m);
}