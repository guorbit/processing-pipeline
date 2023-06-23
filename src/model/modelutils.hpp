
#ifndef MODELUTILS_H
#define MODELUTILS_H
#include <tensorflow/c/c_api.h>
TF_Graph * loadModel(const char *modelPath);

void predictModel(TF_Graph *graph, TF_Status *status, TF_Session *session, TF_Output *inputs, TF_Output *outputs, TF_Tensor *input_values, TF_Tensor *output_values);

#endif // MODELUTILS_H
