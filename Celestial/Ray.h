

#ifndef PRAO_FOURIER_RAY_H
#define PRAO_FOURIER_RAY_H


#include <map>
#include <vector>
#include <complex>

class Ray {
private:
    std::vector<float> complexAmplitudes;
    std::vector<float> rawData;
    int number;
    std::vector<float> modulus;
    std::vector<float> rawDataAverage;
    void calculateModulus();
    void calculateRawDataAverage();
    int nbands;
public:
    Ray() = default;
    Ray(int ray_num, int nbands): number(ray_num), nbands(nbands) {}

    std::vector<float> &getRayModulus();

    void setRawData(const std::vector<float> &rawData);

    void setComplexAmplitudes(const std::vector<float> &complexAmplitudes);

    std::vector<float> &getRawDataAverage();
};


#endif //PRAO_FOURIER_RAY_H
