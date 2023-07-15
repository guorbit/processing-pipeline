//
// IModel.h
//
#include <tensorflow/c/c_api.h>

class IModel
{
public:
    virtual ~IModel() {}
    virtual void loadModel(const char *modelPath) = 0;
    virtual void predict(unsigned char* image, int height, int width, int channels) = 0;
    static void deallocator(void* data, size_t length, void* arg);
};