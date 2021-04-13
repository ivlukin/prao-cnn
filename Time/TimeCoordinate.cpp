


#include "TimeCoordinate.h"

void TimeCoordinate::generateStarTimes(double beginStarTime, int observationLength) {
    timeCoordinatesWithSameStarTime = std::vector<double>();
    double currentStarTime = beginStarTime;
    // во избежание двойной конверсии, здесь используются звездное время,
    // в солнечное время переводится уже при считывании
    double starTimeStep = 24 * 3600;
    for (int i = 0; i < observationLength; ++i) {
        timeCoordinatesWithSameStarTime.push_back(currentStarTime);
        currentStarTime += starTimeStep;
    }
}

TimeCoordinate::TimeCoordinate(double beginStarTime, int observationLength) {
    this->beginDateTime = beginStarTime;
    // generate related star times with step 1 star day
    generateStarTimes(beginStarTime, observationLength);
}

const std::vector<double> &TimeCoordinate::getTimeCoordinatesWithSameStarTime() const {
    return timeCoordinatesWithSameStarTime;
}

double TimeCoordinate::getBeginDateTime() const {
    return beginDateTime;
}
