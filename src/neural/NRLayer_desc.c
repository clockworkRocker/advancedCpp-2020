#include "NRLayer_desc.h"
#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* ------------ LAYER FUNCTIONS ------------ */

/* ---- Layer shutdown ---- */
// - Frees all the memory in Dense function in order to make a safe exit
void layer_shutdown(FILE *file, Matrix *matrix, Vector *vector) {
  fclose(file);
  Matrix_delete(matrix);
  Vector_delete(vector);
}

/* ---- Dense ---- */
int dense(NRLayer *description, Matrix *input) {
  FILE *layer_matrix_file;                           // File to read the matrix W and vector b from
  Matrix W;                                          // Matrix of weights
  Matrix result;                                     // Matrix which is the result of the multiplication
  Vector b;                                          // Additional vector
  int error = 0;                                     // Error marker

  if (description->type != NR_DENSE)                 // Prevent working with wrong layer description
    return -1;

  layer_matrix_file = fopen(description->name, "r");
  if (layer_matrix_file == NULL)                     // Check opened file
    return -1;

  Matrix_init(&W, description->output_dim, description->input_dim);
  Matrix_init(&result, description->output_dim, input->width);
  Matrix_nullify(&result);
  Vector_init(&b, description->output_dim);

  if (Matrix_fill_from_stream(&W, layer_matrix_file) < 0) {
    layer_shutdown(layer_matrix_file, &W, &b);
    return -1;
  }

  if (Vector_fill_from_stream(&b, layer_matrix_file) < 0) {
    layer_shutdown(layer_matrix_file, &W, &b);
    return -1;
  }

  if (Matrix_multiply(input, &W, &result) < 0)
    error = -1;

  if (Matrix_add_Vector(&result, &b) < 0)
    error = -1;

  Matrix_delete(input);
  Matrix_delete(&W);
  Vector_delete(&b);

  input->length = result.length;
  input->width = result.width;
  input->cell = result.cell;

  return error;
}

/* ---- ReLu ---- */
int relu(NRLayer* description, Matrix* input){
  if (description == NULL || input == NULL)
    return -1;
  if (description->type != NR_RELU)
    return -1;

  int i, j;
  for (i = 0; i < input->width; ++i)
    for (j = 0; j < input->length; ++j)
      input->cell[i][j] = MAX(input->cell[i][j], 0);

  return 0;
}