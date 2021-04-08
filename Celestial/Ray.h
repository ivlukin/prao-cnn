

#ifndef PRAO_FOURIER_RAY_H
#define PRAO_FOURIER_RAY_H


#include <map>
#include <vector>

class Ray {
private:
    std::map<int, std::vector<float>> bandMap;
    int number;
    std::vector<float> bandSummary;
    std::vector<float> average;
    void calculateBandSummary();
public:
    Ray() = default;
    explicit Ray(int ray_num): number(ray_num) {}

    std::vector<float> &getBandSummary();

    void setBandMap(const std::map<int, std::vector<float>> &bandMap) {
        Ray::bandMap = bandMap;
    }

    std::vector<float> &getBandAverage();
};


#endif //PRAO_FOURIER_RAY_H
