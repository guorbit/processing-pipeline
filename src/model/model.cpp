#include <fstream>
#include <iostream>
#include "model.hpp"


void model::loadModel(const char* modelPath){
    printf("Loading model from %s\n", modelPath);
    TF_Graph* graph = TF_NewGraph();
    TF_Status* status = TF_NewStatus();
    TF_SessionOptions* session_options = TF_NewSessionOptions();
    TF_Buffer* run_options = TF_NewBufferFromString("", 0);
    TF_ImportGraphDefOptions* graph_opts = TF_NewImportGraphDefOptions();

    // Read the SavedModel file
    std::ifstream model_file("path/to/model.pb", std::ios::binary);
    std::string model_data((std::istreambuf_iterator<char>(model_file)), std::istreambuf_iterator<char>());
    TF_Buffer* model_buffer = TF_NewBufferFromString(model_data.data(), model_data.size());

    // Load the model into the graph
    TF_GraphImportGraphDef(graph, model_buffer, graph_opts, status);

    // Create a session
    TF_Session* session = TF_NewSession(graph, session_options, status);
    if (TF_GetCode(status) != TF_OK) {
        fprintf(stderr, "ERROR: Unable to import graph %s", TF_Message(status));
        return;
    }else{
        printf("Model loaded successfully\n");
    }
    model::graph = graph;
    model::status = status;
    model::sess_opts = session_options;
    model::session = session;
    model::run_opts = run_options;
}

void model::predict(){
    // Set input tensor
    TF_Tensor* input_tensor = nullptr;
    TF_Output input_op = {TF_GraphOperationByName(graph, "input_tensor_name"), 0};

    // Set output tensor
    TF_Tensor* output_tensor = nullptr;
    TF_Output output_op = {TF_GraphOperationByName(graph, "output_tensor_name"), 0};

    // Run the session
    TF_SessionRun(session, run_opts, &input_op, &input_tensor, 1, &output_op, &output_tensor, 1, nullptr, 0, nullptr, status);
}

model::model(/* args */)
{
}

model::~model()
{
    TF_DeleteGraph(graph);
    TF_DeleteStatus(status);
    TF_DeleteSessionOptions(sess_opts);
    TF_DeleteSession(session, status);
    TF_DeleteBuffer(run_opts);
}