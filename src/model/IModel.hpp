//
// IModel.h
//

class IModel
{
public:
    virtual ~IModel() {}
    virtual void loadModel(const char *modelPath) = 0;
    virtual void predict(unsigned char* image, int height, int width, int channels) = 0;
};