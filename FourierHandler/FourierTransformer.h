
#ifndef PRAO_CLASSIFIER_FOURIERTRANSFORMER_H
#define PRAO_CLASSIFIER_FOURIERTRANSFORMER_H


#include "../Context/OpenCLContext.h"
#include "Processor.h"
#include "clFFT.h"
#include "cmath"

#define POINT_TYPE float
class FourierTransformer : Processor {
private:

    cl_mem inputBuffer{};
    cl_mem fftBuffer{};
    int arraySize{};
    POINT_TYPE *array{};
    POINT_TYPE *result{};

    size_t bufferSize{};
    size_t outBufferSize{};

    clfftPlanHandle planHandle{}; // параметры для преобразования Фурье. инициализируется



public:
    POINT_TYPE *getResult() const {
        return result;
    }



private:
    void initBuffers();

    void preparePlan();


public:
    explicit FourierTransformer(OpenCLContext context, POINT_TYPE *array, size_t arraySize) : Processor(context) {
        this->array = array;
        this->arraySize = arraySize;
        this->bufferSize = arraySize * sizeof(POINT_TYPE);
        this->outBufferSize = (arraySize / 2 + 1) * sizeof(POINT_TYPE) * 2;
        preparePlan();
        initBuffers();
    }

    void transform();

    void releaseResources();


};


#endif //PRAO_CLASSIFIER_FOURIERTRANSFORMER_H
