class IFilter
{
public:
    virtual ~IFilter() {}
    virtual void doProcessing() = 0;
    virtual void doDecision() = 0;
};