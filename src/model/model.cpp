#include <fstream>
#include <iostream>
#include "model.hpp"

void Model::loadModel(const char *modelPath)
{
    printf("Loading model from %s\n", modelPath);
    TF_Graph *graph = TF_NewGraph();
    TF_Status *status = TF_NewStatus();
    TF_SessionOptions *session_options = TF_NewSessionOptions();
    TF_Buffer *run_options = TF_NewBufferFromString("", 0);
    TF_ImportGraphDefOptions *graph_opts = TF_NewImportGraphDefOptions();

    std::ifstream model_file(modelPath, std::ios::binary);
    if (!model_file.is_open())
    {
        fprintf(stderr, "ERROR: Unable to open model file %s\n", modelPath);
        return;
    }
    std::string model_data((std::istreambuf_iterator<char>(model_file)), std::istreambuf_iterator<char>());
    TF_Buffer *model_buffer = TF_NewBufferFromString(model_data.data(), model_data.size());

    // Load the model into the graph
    TF_GraphImportGraphDef(graph, model_buffer, graph_opts, status);

    // Create a session
    TF_Session *session = TF_NewSession(graph, session_options, status);
    if (TF_GetCode(status) != TF_OK)
    {
        fprintf(stderr, "ERROR: Unable to import graph %s", TF_Message(status));
        return;
    }
    else
    {
        printf("Model loaded successfully\n");
    }
    Model::graph = graph;
    Model::status = status;
    Model::sess_opts = session_options;
    Model::session = session;
    Model::run_opts = run_options;
}

void Model::predict(unsigned char *image, int height, int width, int channels)
{

    printf("Predicting on graph...\n");
    if (graph == nullptr)
    {
        printf("ERROR: Graph not initialized\n");
        return;
    }
    // Set input tensor
    TF_DataType dtype = TF_UINT8;
    int64_t dims[] = {1, height, width, channels};
    size_t ndims = 4;

    TF_Tensor *input_tensor = TF_NewTensor(dtype, dims, ndims, image, height * width * channels * sizeof(unsigned char), &Model::deallocator, 0);
    TF_Output input_op = {TF_GraphOperationByName(graph, "x"), 0};

    // Set output tensor
    TF_Tensor *output_tensor = nullptr;
    TF_Output output_op = {TF_GraphOperationByName(graph, "sequential/output_layer/Softmax"), 0};

    printf("Running session...\n");
    if (session == nullptr)
    {
        printf("ERROR: Session not initialized\n");
        return;
    }
    if (status == nullptr)
    {
        printf("ERROR: Status not initialized\n");
        return;
    }


    // Run the session
    TF_SessionRun(session, run_opts, &input_op, &input_tensor, 1, &output_op, &output_tensor, 1, nullptr, 0, nullptr, status);
}
void Model::deallocator(void *data, size_t length, void *arg)
{
    free(data);
}

void Model::Delete()
{
    Model::~Model();
}

Model::Model(/* args */)
{
    printf("Model initialized!\n");
}

Model::~Model()
{

    if (graph)
        TF_DeleteGraph(graph);
    if (session)
        TF_DeleteSession(session, status);
    if (status)
        TF_DeleteStatus(status);
    if (sess_opts)
        TF_DeleteSessionOptions(sess_opts);
    if (run_opts)
        TF_DeleteBuffer(run_opts);
    printf("Model destroyed!\n");
}