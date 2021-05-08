
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
    std::map<int, std::vector<double>> summaryOfRawDataForEveryRayInTime;
    bool isCalculated = false;
    bool isRawCalculated = false;
    void calculateSummary();
    void calculateRawSummary();
public:
    explicit SummationHandler(std::vector<Timestamp> relatedTimestamps) {
        this->relatedTimestamps = std::move(relatedTimestamps);
    }
    SummationHandler() = default;

    std::map<int, std::vector<double>> &getSummaryForEveryRayInTime() {
        if (!isCalculated)
            calculateSummary();
        return summaryForEveryRayInTime;
    }

    std::map<int, std::vector<double>> &getSummaryOfRawDataForEveryRayInTime() {
        if (!isRawCalculated)
            calculateRawSummary();
        return summaryOfRawDataForEveryRayInTime;
    }


};


#endif //PRAO_FOURIER_SUMMARIZEHANDLER_H
