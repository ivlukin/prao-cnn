
#include "SummationHandler.h"

void SummationHandler::calculateSummary() {
    int size = relatedTimestamps[0].getRayMap()[0].getRayModulus().size();
    summaryForEveryRayInTime = std::map<int, std::vector<double>>();
    for (int i = 0; i < 48; ++i) {
        summaryForEveryRayInTime[i] = std::vector<double>(size);
        std::fill(summaryForEveryRayInTime[i].begin(), summaryForEveryRayInTime[i].end(), 0);
    }

    for (Timestamp timestamp: relatedTimestamps) {
        for (int i = 0; i < 48; i++) {
            Ray ray = timestamp.getRayMap()[i];
            for (int j = 0; j < ray.getRayModulus().size(); j++) {
                summaryForEveryRayInTime[i][j] += ray.getRayModulus()[j];
            }
        }
    }

    isCalculated = true;
}

void SummationHandler::calculateRawSummary() {
    int size = relatedTimestamps[0].getRayMap()[0].getRawDataAverage().size();
    summaryOfRawDataForEveryRayInTime = std::map<int, std::vector<double>>();
    for (int i = 0; i < 48; ++i) {
        summaryOfRawDataForEveryRayInTime[i] = std::vector<double>(size);
        std::fill(summaryOfRawDataForEveryRayInTime[i].begin(), summaryOfRawDataForEveryRayInTime[i].end(), 0);
    }

    for (Timestamp timestamp: relatedTimestamps) {
        for (int i = 0; i < 48; ++i) {
            Ray ray = timestamp.getRayMap()[i];
            for (int j = 0; j < ray.getRawDataAverage().size(); ++j) {
                summaryOfRawDataForEveryRayInTime[i][j] += ray.getRawDataAverage()[j];
            }
        }
    }
    isRawCalculated = true;
}
