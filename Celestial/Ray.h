

#ifndef PRAO_FOURIER_RAY_H
#define PRAO_FOURIER_RAY_H


#include <map>
#include <vector>
#include <complex>

class Ray {
private:
    std::vector<float> complexAmplitudes;
    int number;
    std::vector<float> modulus;
    void calculateBandSummary();
    int nbands;
public:
    Ray() = default;
    Ray(int ray_num, int nbands): number(ray_num), nbands(nbands) {}

    std::vector<float> &getRayModulus();

    void setComplexAmplitudes(const std::vector<float> &complexAmplitudes);
};


#endif //PRAO_FOURIER_RAY_H
