

#include "Ray.h"


std::vector<float> &Ray::getRayModulus() {
    if (this->modulus.empty())
        calculateModulus();
    return modulus;
}

void Ray::calculateModulus() {
    int size = complexAmplitudes.size();
    modulus = std::vector<float>();

    modulus.push_back(0);
    for (int j = 2; j < size - 1; j += 2) {
        float real = complexAmplitudes[j] / (float) nbands;
        float imaginary = complexAmplitudes[j + 1] / (float) nbands;
        modulus.push_back(std::sqrt(real * real + imaginary * imaginary));
    }
}


void Ray::setComplexAmplitudes(const std::vector<float> &complexAmplitudes) {
    Ray::complexAmplitudes = complexAmplitudes;
}
