#include "NRProcess.h"
#include <string.h>

int NR_PA_check_args(FILE *stream, const char *path, NRLayerVector *description) {
  if (stream == NULL || path == NULL || description == NULL)
    return -1;
  if (description->mem == NULL)
    return -1;

  return 0;
}

int NR_PA_find_type(char *str, char *buf) {
  if (str == NULL || buf == NULL)
    return -1;

  char c;
  int i = 0;

  for (c = str[i]; c != '(';) {
    buf[i] = c;
    c = str[++i];
  }

  return 0;
}

int NR_PA_check_type(char *str, NRLayer *layer) {

  if (strcmp(str, "Dense") == 0)
    layer->type = NR_DENSE;
  else if (strcmp(str, "ReLU") == 0)
    layer->type = NR_RELU;
  else
    return -1;

  return 0;
}

int NR_PA_find_name(char *str, char *buf) {
  if (str == NULL || buf == NULL)
    return -1;

  int i = 0;
  int j = 0;
  char c = 0;

  for (; c != '\"'; ++i)
    c = str[i];

  for (c = str[i]; c != '\"'; ++j) {
    buf[j] = c;
    c = str[++i];
  }

  return 0;
}

int NR_PA_find_dimensions(char *str, NRLayer *layer) {
  char c = 0;
  int i = 0;

  while (c != ',')                                     // Find the first comma
    c = str[i++];

  if (sscanf(str+i, "%lu, %lu", &(layer->input_dim), &(layer->output_dim)) != 2)
    return -1;

  return 0;
}

int NR_parse_architecture(char *str, NRLayer *layer, const char *path) {
  const int BUF_SIZE = 16;

  char buf[BUF_SIZE];
  int i = 0;
  for (; i < BUF_SIZE; ++i)
    buf[i] = '\0';

  if (NR_PA_find_type(str, buf) < 0)                  // Parse the type part of the string
    return -1;
  if (NR_PA_check_type(buf, layer) < 0)               // Write it to the layer struct
    return -1;

  if (layer->type == NR_DENSE) {

    if (NR_PA_find_name(str, buf) < 0)                  // Parse the part in quotation marks
      return -1;
    strcpy(layer->name, path);                          // Copy the path of the file to the name string
    strcat(layer->name, buf);                           // Add the parsed name to the string

    if (NR_PA_find_dimensions(str, layer) < 0)
      return -1;

  } else {
    strcpy(layer->name,"ReLU");
    layer->input_dim = 0;
    layer->output_dim = 0;
  }
  return 0;
}

int NR_create_architecture(FILE *stream, const char *path, NRLayerVector *description) {
  const int BUF_SIZE = 64;
  const int SAFETY_BUFFER = 16;

  char buf[BUF_SIZE];
  NRLayer tmp;

  int error = 0;

  if (NR_PA_check_args(stream, path, description) < 0)
    return -1;

  tmp.name = calloc(strlen(path) + SAFETY_BUFFER, 1);

  for (; feof(stream) == 0 && error == 0;) {
    if (fgets(buf, BUF_SIZE, stream) == NULL)       // Read one line of architecture file
      error = -1;
    if (NR_parse_architecture(buf, &tmp, path) < 0)     // Parse it and fill the temporary layer struct
      error = -1;
    if (NRLayerVector_push(description, &tmp) < 0)      // Write the information to the main vector
      error = -1;
  }

  NRLayer_delete(&tmp);
  return error;
}
