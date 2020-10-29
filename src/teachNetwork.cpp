#include <iostream>
#include <random>

extern "C" {
#include "NRProcess.h"
}

void createInputFile(NRLayer* layer, FILE* layer_file){
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> uid(-1., 1.);

  layer_file = fopen(layer->name, "w");

  for (size_t i = 0; i < layer->input_dim; ++i) {         // Generate layer matrix
    for (size_t j = 0; j < layer->output_dim; ++j)
      fprintf(layer_file, "%lf ", uid(gen));
    fputc('\n', layer_file);
  }

  fputc('\n', layer_file);

  for(size_t j = 0; j < layer->output_dim; ++j)
    fprintf(layer_file, "%lf", uid(gen));

  fclose(layer_file);
}


int main() {
  FILE *description_file;
  FILE *layer_file = nullptr;
  NRLayerVector layer_list;
  std::string path(INPUT_PATH);
  std::string description_path = path + "description";

  description_file = fopen(description_path.c_str(), "r");
  if(description_file == nullptr)
    return -1;

  NRLayerVector_init(&layer_list);

  NR_create_architecture(description_file, path.c_str(), &layer_list);

  for (size_t i = 0; i < layer_list.length; ++i)
    createInputFile(layer_list.mem + i, layer_file);

  fclose(description_file);
  NRLayerVector_delete(&layer_list);
}