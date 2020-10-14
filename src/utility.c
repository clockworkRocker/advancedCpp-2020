#include <ctype.h>
#include <string.h>
#include "../include/ht1.h"

char memchk(void *ptr) {
  if (ptr == NULL) {
    printf("[Error] No memory allocated!\n");
    exit(3);
  }
  return 0;
}

void bufReset(int *n, char *buf) {
  int i;

  for (i = 0; i < BUF_SIZE; i++)
    buf[i] = '\0';
  *n = -1;
}

int checkForIdentifier(const char *buf, int n) {
  int i;
  int hasLetter = 0;
  int hasDigit = 0;
  int hasUnderscore = 0;
  int _4a;

  if (n <= 1)
    return 0;

  for (i = 0; i < n; i++) {
    int c = buf[i];
    if ((isalpha(c) == 0) && (isdigit(c) == 0) && (c != '_'))
      return 0;

    if(isalpha(c)) {
      hasLetter = 1;
      if(hasDigit)
        return 0;
    }

    if(hasLetter && (isdigit(c)))
      hasDigit = 1;
  }

  if(hasLetter == 0)
    return 0;

  return 1;
}

void collectPyFunctionsFromFile(FILE *file, pyVector *functions) {
  int c;
  char buf[BUF_SIZE];
  int nchar = 0;
  int line = 1;

  char insideFunctionDefinition = 0;
  char insideParameters = 0;

  for (c = fgetc(file); c != EOF; nchar++) {
    buf[nchar] = c;
    switch (c) {
      case '(':
        if (checkForIdentifier(buf, nchar - 1)) {
          int pos;
          insideFunctionDefinition++;

          buf[nchar] = '\0';
          pos = vectorFindName(functions, buf);

          if (pos != -1)
            intVectorPush(&(functions->array[pos].lines), line);

          else {
            vectorPush(functions, buf, line);
            insideParameters = 1;
          }
        }

        bufReset(&nchar, buf);
        break;

      case ')':
        if (insideFunctionDefinition > 0) {
          insideFunctionDefinition--;
          insideParameters = 0;
        }
        break;

      case ',':
        if (insideParameters) {
          functions->array[functions->takenSpots - 1].n_parameters++;
        }

        bufReset(&nchar, buf);
        break;

      case '\n':line++;
      case ' ':
      case '.':
      case '+':
      case '-':
      case '/':
      case '*':
      case '=':
      case '\t':bufReset(&nchar, buf);
        break;

      default:
        if (insideParameters && (functions->array[functions->takenSpots - 1].n_parameters == 0)) {
          functions->array[functions->takenSpots - 1].n_parameters++;
        }

    }
    c = fgetc(file);
  }
}


