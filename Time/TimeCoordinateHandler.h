

#ifndef PRAO_FOURIER_TIMECOORDINATEHANDLER_H
#define PRAO_FOURIER_TIMECOORDINATEHANDLER_H


#include <string>
#include "../Config/Config.h"
#include "../Reader//DataHeader.h"
#include "TimeCoordinate.h"
#include <ctime>
#include <sstream>
#include <iomanip>
#include <vector>
#include "TimeUtils.h"
#include "../Reader/FilesListItem.h"
#include "../Utils/Utils.h"

class TimeCoordinateHandler {
private:
    tm startDateSun{};
    tm endDateSun{};
    int observationLength;
    int step;
    std::vector<TimeCoordinate> timeCoordinateSet;
    FilesListItem firstFile;
    std::string fileListPath;
    std::string range;
    std::string mode;
    bool hasEndDate = false;

private:
    tm getDateTimeFromString(const std::string &dateTimeAsString);

    bool scanForFileItem(const std::string& fileName);

public:
    explicit TimeCoordinateHandler(const Config &config);

    void generateTimeCoordinates();

    const std::vector<TimeCoordinate> &getTimeCoordinateSet() const {
        return timeCoordinateSet;
    }
};


#endif //PRAO_FOURIER_TIMECOORDINATEHANDLER_H
