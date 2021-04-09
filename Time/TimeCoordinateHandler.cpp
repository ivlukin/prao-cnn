

#include "TimeCoordinateHandler.h"



TimeCoordinateHandler::TimeCoordinateHandler(const Config &config) {
    this->startDateSun = getDateTimeFromString(config.getStartDate());
    this->observationLength = config.getObservationLength();
    this->step = config.getStep();
    this->fileListPath = config.getFileListPath();
    this->range = config.getRange();
    this->mode = config.getMode();
}

std::tm TimeCoordinateHandler::getDateTimeFromString(const std::string &dateTimeAsString) {
    std::tm dateTime = {};
    std::istringstream ss(dateTimeAsString);
    if (ss >> std::get_time(&dateTime, "%Y-%m-%d %H:%M:%S")) {
        std::put_time(&dateTime, "%c");
    }
    return dateTime;
}


void TimeCoordinateHandler::generateTimeCoordinates() {
    // all time coordinates will be stored epoch time (and timezone doesn't matter)
    int year = startDateSun.tm_year;
    int month = startDateSun.tm_mon;
    if (year < 200) {
        year += 1900;
    }
    int mday = startDateSun.tm_mday;
    int hour = startDateSun.tm_hour;

    std::string searchingFile = Utils::getFileFromDate(year, month, mday, hour, range, mode);
    bool found = scanForFileItem(searchingFile);
    if (!found) {
        throw std::logic_error("couldn't find entry in fileListPath of startDate");
    }

    double startDateTimeLocal = to_starTime(mktime(&this->startDateSun));
    startDateTimeLocal += firstFile.star_time_start;
    int numIterations = (int) (to_SunTime(3600 * 24) / this->step);
    for (int i = 0; i < numIterations; ++i) {
        TimeCoordinate timeCoordinate = TimeCoordinate(startDateTimeLocal, observationLength);
        timeCoordinateSet.push_back(timeCoordinate);
        startDateTimeLocal += this->step;
    }

}

bool TimeCoordinateHandler::scanForFileItem(const std::string &fileName) {
    ifstream in(this->fileListPath);
    if (!in.good())
        throw std::logic_error(fileListPath + " not found!");
    while (!in.eof()) {
        FilesListItem item;
        in >> item;
        if (item.filename == fileName) {
            firstFile = item;
            return true;
        }
    }
    in.close();
    return false;
}


