#include "TFLiteModel.hpp"

void TFLiteModel::loadModel(const char *modelPath)
{
    TFLiteModel::logger -> log("Loading model from %s", modelPath);
    TF_Graph *graph = TF_NewGraph();
    TF_Status *status = TF_NewStatus();
    TF_SessionOptions *session_options = TF_NewSessionOptions();
    TF_Buffer *run_options = TF_NewBufferFromString("", 0);
    TF_ImportGraphDefOptions *graph_opts = TF_NewImportGraphDefOptions();

    std::ifstream model_file(modelPath, std::ios::binary);
    if (!model_file.is_open())
    {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        TFLiteModel::logger -> log(level,"Unable to open model file %s", modelPath);
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
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        TFLiteModel::logger -> log(level,"Unable to import graph %s", TF_Message(status));
        return;
    }
    else
    {
        TFLiteModel::logger -> log("Graph loaded succesfully");
    }
    TFLiteModel::graph = graph;
    TFLiteModel::status = status;
    TFLiteModel::sess_opts = session_options;
    TFLiteModel::session = session;
    TFLiteModel::run_opts = run_options;
    TF_DeleteImportGraphDefOptions(graph_opts);
    TF_DeleteBuffer(model_buffer);
}

void TFLiteModel::predict(unsigned char *image, int height, int width, int channels)
{

    TFLiteModel::logger -> log("Performing inference");
    if (graph == nullptr)
    {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        TFLiteModel::logger -> log(level, "Graph not initialized");
        return;
    }
    // Set input tensor
    TF_DataType dtype = TF_UINT8;
    int64_t dims[] = {1, height, width, channels};
    size_t ndims = 4;

    TF_Tensor *input_tensor = TF_NewTensor(dtype, dims, ndims, image, height * width * channels * sizeof(unsigned char), &TFLiteModel::deallocator, 0);
    TF_Output input_op = {TF_GraphOperationByName(graph, "x"), 0};

    // Set output tensor
    TF_Tensor *output_tensor = nullptr;
    TF_Output output_op = {TF_GraphOperationByName(graph, "sequential/output_layer/Softmax"), 0};

    if (session == nullptr)
    {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        TFLiteModel::logger -> log(level,"TF object session not initialized");
        return;
    }
    if (status == nullptr)
    {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        TFLiteModel::logger -> log(level,"TF object status not initialized");
        return;
    }

    this -> logger -> log("Running session");
    // Run the session
    if (session)
        TF_SessionRun(session, run_opts, &input_op, &input_tensor, 1, &output_op, &output_tensor, 1, nullptr, 0, nullptr, status);
    this -> logger -> log("Session run");
    if (input_tensor)
        TF_DeleteTensor(input_tensor);
    
}
void TFLiteModel::deallocator(void *data, size_t length, void *arg)
{
    // free(data);
}

// void Model::Delete()
// {
//     Model::~Model();
// }

TFLiteModel::TFLiteModel(ThreadLogger * logger)
{
    TFLiteModel::logger = logger;
    logger -> log("TF lite module initialized");
}

TFLiteModel::~TFLiteModel()
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
    TFLiteModel::logger -> log("TF Lite module destroyed");
}