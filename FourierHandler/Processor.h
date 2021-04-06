
#ifndef PRAO_CLASSIFIER_PROCESSOR_H
#define PRAO_CLASSIFIER_PROCESSOR_H


#include "../Context/OpenCLContext.h"
#include <iostream>

class Processor {
protected:
    OpenCLContext context = OpenCLContext();

    explicit Processor(OpenCLContext context) {
        this->context = context;
    }

    void printError(const char message[], int clError);
};


#endif //PRAO_CLASSIFIER_PROCESSOR_H
