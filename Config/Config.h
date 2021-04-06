

#ifndef PRAO_FOURIER_CONFIG_H
#define PRAO_FOURIER_CONFIG_H


#include <string>
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/stringbuffer.h"
#include "../rapidjson/filereadstream.h"
#include <iostream>

class Config {
public:
    explicit Config(char* fileName);
private:
    std::string startDate;
    std::string endDate;
    std::string range;
    int step;
    std::string outputPath;
    std::string mode;
    std::string fileListPath;
    std::string calibrationListPath;
    double durationStarSeconds;
public:
    const std::string &getStartDate() const;

    const std::string &getEndDate() const;


    const std::string &getRange() const;

    int getStep() const;

    const std::string &getOutputPath() const;

    const std::string &getMode() const;

    const std::string &getFileListPath() const;

    const std::string &getCalibrationListPath() const;

    double getDurationStarSeconds() const;
};


#endif //PRAO_FOURIER_CONFIG_H
