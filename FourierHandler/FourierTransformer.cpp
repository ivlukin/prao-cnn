
#include "FourierTransformer.h"

void FourierTransformer::transform() {
    int clError;

    //write raw data to buffer
    clError = clEnqueueWriteBuffer(context.getClCommandQueue(), inputBuffer, CL_TRUE, 0, this->bufferSize, array, 0,
                                   nullptr, nullptr);
    printError("error writing data to buffer", clError);

    //execute the plan
    clError = clfftEnqueueTransform(planHandle, CLFFT_FORWARD, 1, &context.getClCommandQueue(), 0, nullptr, nullptr,
                                    &inputBuffer, &fftBuffer,
                                    nullptr);

    printError("error executing the fft!", clError);

    result = new float[outBufferSize / sizeof(POINT_TYPE)];
    clError = clEnqueueReadBuffer(context.getClCommandQueue(), fftBuffer, CL_TRUE, 0,
                                  this->outBufferSize, result, 0,
                                  nullptr, nullptr);
    printError("error reading fft buffer!", clError);

}

void FourierTransformer::initBuffers() {
    int clError;


    /* input buffer. array of arrays */
    inputBuffer = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE,
                                 this->bufferSize, nullptr, &clError);
    printError("error creating input buffer", clError);

    /* out buffer. array of complex amplitudes in the format RIRIRI (real imaginary) */

    fftBuffer = clCreateBuffer(context.getContext(), CL_MEM_READ_WRITE, this->outBufferSize, nullptr, &clError);
    printError("error creating fft buffer", clError);
}

void FourierTransformer::preparePlan() {
    int clError;

    clfftDim dim = CLFFT_1D;
    size_t clLengths[1] = {static_cast<size_t>(arraySize)};

    clfftSetupData fftSetup;
    fftSetup.debugFlags = 0;
    fftSetup.major = clfftVersionMajor;
    fftSetup.minor = clfftVersionMinor;
    fftSetup.patch = clfftVersionPatch;

    clError = clfftSetup(&fftSetup);
    printError("error data setup!", clError);

    // создание плана
    clError = clfftCreateDefaultPlan(&planHandle, context.getContext(), dim, clLengths);
    printError("error creating default fft plan!", clError);

    //настройка параметров
    clError = clfftSetPlanPrecision(planHandle, CLFFT_SINGLE);
    clError = clfftSetLayout(planHandle, CLFFT_REAL, CLFFT_HERMITIAN_INTERLEAVED);
    clError = clfftSetResultLocation(planHandle, CLFFT_OUTOFPLACE);

    clError = clfftBakePlan(planHandle, 1, &context.getClCommandQueue(), nullptr, nullptr);
    printError("error baking the plan!", clError);
}

void FourierTransformer::releaseResources() {
    clReleaseMemObject(inputBuffer);
    clReleaseMemObject(fftBuffer);
    clfftDestroyPlan(&planHandle);
}

