//
// Created by sorrow on 03.09.2020.
//

#ifndef PRAO_FOURIER_TIMECOORDINATE_H
#define PRAO_FOURIER_TIMECOORDINATE_H


#include <string>
#include <vector>
#include "TimeUtils.h"

class TimeCoordinate {
private:
    double beginDateTime;
    std::vector<double> timeCoordinatesWithSameStarTime;

private:
   void generateStarTimes(double beginStarTime, double endStarTime);
public:
    TimeCoordinate(double beginStarTime, double endStartTime);

    TimeCoordinate() = default;

    const std::vector<double> &getTimeCoordinatesWithSameStarTime() const;

    double getBeginDateTime() const;

};


#endif //PRAO_FOURIER_TIMECOORDINATE_H
