#include "pipeline.hpp"



int main() {
    Pipeline * pipeline = new Pipeline();
    pipeline -> run();
    delete pipeline;
    return 0;
}

