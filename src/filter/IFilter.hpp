//
// IFilter.h
//
class IFilter
{
public:
    virtual ~IFilter() {}
    virtual int * doProcessing(unsigned char* image, int width, int height, int channels) = 0;
    virtual void doDecision() = 0;
    virtual void process(InputType input) = 0;
};
