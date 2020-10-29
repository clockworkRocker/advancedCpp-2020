#include "NRProcess.h"
#include <string.h>

void resource_free(FILE *input, FILE *description, FILE *output, Matrix *matrix, NRLayerVector *layer_vector) {
  fclose(input);
  fclose(description);
  fclose(output);

  Matrix_delete(matrix);
  free(matrix);

  NRLayerVector_delete(layer_vector);
}

int main(int argc, char *argv[]) {
  FILE *input_file = NULL;
  FILE *output_file = NULL;
  FILE *description_file = NULL;
  NRLayerVector description;
  Matrix *matrix = NULL;
  size_t n_objects, n_qualities;

  char *description_path;
  char *description_file_path;

  if (argc < 4) {
    puts("[ Usage ] ./Task2 <input matrix file> <neural network files directory> <output matrix file>");
    return 1;
  }

  input_file = fopen(argv[1], "r");
  if (input_file == NULL) {
    printf("[File] Couldn't open file %s\n", argv[1]);
    return -1;
  }

  description_path = argv[2];

  description_file_path = malloc(strlen(description_path) + strlen("description") + 1);

  stpcpy(description_file_path, description_path);
  strcat(description_file_path, "description");

  description_file = fopen(description_file_path, "r");
  if (description_file == NULL){
    printf("[File] Couldn't open file %s\n", description_file_path);
    fclose(input_file);
    return -1;
  }

  output_file = fopen(argv[3], "w");
  if (output_file == NULL) {
    printf("[File] Couldn't open file %s\n", argv[3]);
    fclose(description_file);
    fclose(input_file);
    return -1;
  }

  NRLayerVector_init(&description);
  if (NR_create_architecture(description_file, description_path, &description) < 0) {
    resource_free(input_file, description_file, output_file, matrix, &description);
    return -1;
  }

  matrix = malloc(sizeof(Matrix));
  fscanf(input_file,"%lu, %lu", &n_objects, &n_qualities);
  Matrix_init(matrix, n_qualities, n_objects);
  Matrix_fill_from_stream(matrix, input_file);

#ifdef USE_THREADS
  NR_process_parallel(matrix, &description, output_file);
#else
  if(NR_process_linear(matrix, &description, output_file) < 0)
    return -1;
#endif //USE_THREADS

  resource_free(input_file, description_file, output_file, matrix, &description);
  free(description_file_path);

  return 0;
}
