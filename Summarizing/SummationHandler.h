
#ifndef PRAO_FOURIER_SUMMARIZEHANDLER_H
#define PRAO_FOURIER_SUMMARIZEHANDLER_H


#include <utility>
#include <vector>
#include "../Celestial/Timestamp.h"
#include "../Config/Config.h"

class SummationHandler {
private:
    std::vector<Timestamp> relatedTimestamps;
    std::map<int, std::vector<double>> summaryForEveryRayInTime;
    bool isCalculated = false;
public:
    SummationHandler(const Config& config, std::vector<Timestamp> relatedTimestamps) {
        this->relatedTimestamps = std::move(relatedTimestamps);
    }
    SummationHandler() = default;

    std::map<int, std::vector<double>> &getSummaryForEveryRayInTime() {
        if (!isCalculated)
            calculateSummary();
        return summaryForEveryRayInTime;
    }


    void calculateSummary();
};


#endif //PRAO_FOURIER_SUMMARIZEHANDLER_H
