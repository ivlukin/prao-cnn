
#include "Ray.h"


 std::vector<float> &Ray::getBandSummary()  {
    if (this->bandSummary.empty())
        calculateBandSummary();
    return bandSummary;
}

void Ray::calculateBandSummary()  {
    bandSummary = std::vector<float>(bandMap[0].size());
    std::fill(bandSummary.begin(), bandSummary.end(), 0);
    for (auto& bandEntry: bandMap) {
        // последний (суммирующий) бенд считать не нужно
        for (int i = 0; i < bandEntry.second.size() - 1; ++i) {
            bandSummary[i] += bandEntry.second[i];
        }
    }
}
