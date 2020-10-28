#include <string.h>
#include "NRLayer_desc.h"

/* ------------ "CONSTRUCTOR" ------------  */
int NRLayer_init(NRLayer *dest, const char *name, int type, size_t input_dim, size_t output_dim) {
  if (dest == NULL || name == NULL)                 // Check pointer args
    return -1;

  if ((type != NR_DENSE) && (type != NR_RELU))      // Check type
    return -1;

  dest->name = malloc(strlen(name) + 1);
  if (dest->name == NULL)
    return -1;

  strcpy(dest->name, name);
  dest->input_dim = input_dim;
  dest->output_dim = output_dim;
  dest->type = type;

  return 0;
}

/* ------------ "DESTRUCTOR" ------------  */
int NRLayer_delete(NRLayer *dest) {
  if (dest == NULL)
    return -1;

  free(dest->name);

  dest->name = NULL;
  dest->input_dim = 0;
  dest->output_dim = 0;

  return 0;
}

/* ------------ ACTIVATION FUNCTION ------------ */
/* - This function chooses, which layer function to call based on its description - */

int NRLayer_activate(NRLayer *description, Matrix *input) {

  int (*activate[2])(NRLayer *description, Matrix *input) = {dense, relu};

  if (description == NULL || input == NULL)                               // Check pointer args
    return -1;

  if ((description->type != NR_RELU) && (description->type != NR_DENSE)) // Check type
    return -1;

  if (description->type == NR_DENSE && (description->input_dim != input->length))                           // Check dimensions
    return -1;

  if (activate[description->type](description, input) < 0)
    return -1;

  return 0;
}