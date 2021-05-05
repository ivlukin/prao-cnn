

#include "Ray.h"


std::vector<float> &Ray::getRayModulus() {
    if (this->modulus.empty())
        calculateBandSummary();
    return modulus;
}

void Ray::calculateBandSummary() {
    int size = complexAmplitudes.size();
    modulus = std::vector<float>();

    for (int j = 0; j < (size / 2 + 1) * 2 - 1; j += 2) {
        float real = complexAmplitudes[j] / (float) nbands;
        float imaginary = complexAmplitudes[j + 1] / (float) nbands;
        modulus.push_back(std::sqrt(real * real + imaginary * imaginary));
    }
}


void Ray::setComplexAmplitudes(const std::vector<float> &complexAmplitudes) {
    Ray::complexAmplitudes = complexAmplitudes;
}
