#include "pyVector.h"
#include "ht1.h"
#include <string.h>

/*      ======== pyVector functions ========        */

void vectorCopyString(pyVector *vector, int pos, char *buf) {
  if (vector->array[pos].name == NULL)
    vector->array[pos].name = malloc(strlen(buf) + 1);

  memchk(vector->array[pos].name);
  strcpy(vector->array[pos].name, buf);
}

void vectorEnlarge(pyVector *old) {
  pyVector new;

  new.array = realloc(old->array, 2 * old->size * sizeof(PYTHON_FUNC));

  memchk(new.array);

  old->array = new.array;
  old->size *= 2;

}

int vectorFindName(pyVector *vector, char *buf) {
  int i;
  for (i = 0; i < vector->takenSpots; i++)
    if (strcmp(vector->array[i].name, buf) == 0)
      return i;

  return -1;
}

void vectorFree(pyVector *vector) {
  int i;
  if (vector->array == NULL)
    return;

  for (i = 0; i < vector->takenSpots; i++) {
    if (vector->array[i].name != NULL)
      free(vector->array[i].name);
    if (vector->array[i].lines.array != NULL)
      free(vector->array[i].lines.array);
  }

  free(vector->array);

}

void vectorPrint(pyVector *vector) {
  int i;

  for (i = 0; i < vector->takenSpots; i++) {
    printf("%d. Function '%s' is present on lines ", i + 1, vector->array[i].name);
    intVectorPrint(&(vector->array[i].lines));
    printf(" with %d parameters\n", vector->array[i].n_parameters);
  }

}

void vectorPush(pyVector *vector, char *buf, int line) {
  int pos = vector->takenSpots;

  if (pos + 1 > vector->size)
    vectorEnlarge(vector);

  vector->takenSpots++;

  vector->array[pos].name = NULL;
  vectorCopyString(vector, pos, buf);

  vector->array[pos].lines.size = ARR_SIZE;
  vector->array[pos].lines.array = malloc(ARR_SIZE * sizeof(int));
  vector->array[pos].lines.takenSpots = 0;

  vector->array[pos].n_parameters = 0;

  memchk(vector->array[pos].lines.array);

  intVectorPush(&(vector->array[pos].lines), line);

}