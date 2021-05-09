

#include "Ray.h"


std::vector<float> &Ray::getRayModulus() {
    if (this->modulus.empty())
        calculateModulus();
    return modulus;
}

void Ray::calculateModulus() {
    int size = complexAmplitudes.size();
    modulus = std::vector<float>();

    for (int j = 0; j < size - 1; j += 2) {
        float real = complexAmplitudes[j] / (float) nbands;
        float imaginary = complexAmplitudes[j + 1] / (float) nbands;
        modulus.push_back(std::sqrt(real * real + imaginary * imaginary));
    }
}


void Ray::setComplexAmplitudes(const std::vector<float> &complexAmplitudes) {
    Ray::complexAmplitudes = complexAmplitudes;
}

void Ray::setRawData(const std::vector<float> &rawData) {
    Ray::rawData = rawData;
}

void Ray::calculateRawDataAverage() {
    int size = rawData.size();
    rawDataAverage = std::vector<float>();
    for (int i = 0; i < size; ++i)
        rawDataAverage.push_back(rawData[i] / (float) nbands);
}

std::vector<float> &Ray::getRawDataAverage() {
    if (rawDataAverage.empty())
        calculateRawDataAverage();
    return rawDataAverage;
}
