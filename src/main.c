#include "../include/ht1.h"

int main(int argc, char *argv[]) {
  FILE *input;
  pyVector functions = {NULL, ARR_SIZE, 0};

  if (argc < 2) {
    printf("[Info] No filename specified. Setting input to stdin\n");
    input = stdin;
    //return 1;
  } else
    input = fopen(argv[1], "r");

  if (input == NULL) {
    printf("[Error] Could not open file\n");
    return 2;
  }

  vectorInit(&functions);

  collectPyFunctionsFromFile(input, &functions);

  vectorPrint(&functions);

  vectorFree(&functions);

  fclose(input);

  return 0;
}   