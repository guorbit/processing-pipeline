class IFilter
{
public:
    virtual ~IFilter() {}
    virtual void doProcessing(unsigned char* image, int width, int height, int channels) = 0;
    virtual void doDecision() = 0;
};