

#include "TimeCoordinate.h"

void TimeCoordinate::generateStarTimes(double beginStarTime, double endStarTime) {
    timeCoordinatesWithSameStarTime = std::vector<double>();
    double currentStarTime = beginStarTime;
    int numIterations = (int) ((endStarTime - beginStarTime) / to_SunTime(24 * 3600)) + 1;
    double starTimeStep = to_SunTime(24 * 3600);
    for (int i = 0; i < numIterations; ++i) {
        timeCoordinatesWithSameStarTime.push_back(currentStarTime);
        currentStarTime += starTimeStep;
    }
}

TimeCoordinate::TimeCoordinate(double beginStarTime, double endStarTime) {
    this->beginDateTime = beginStarTime;
    // generate related star times with step 1 star day
    generateStarTimes(beginStarTime, endStarTime);
}

const std::vector<double> &TimeCoordinate::getTimeCoordinatesWithSameStarTime() const {
    return timeCoordinatesWithSameStarTime;
}

double TimeCoordinate::getBeginDateTime() const {
    return beginDateTime;
}
