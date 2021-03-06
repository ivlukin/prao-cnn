//
// Created by sorrow on 07.04.2021.
//

#ifndef PRAO_CNN_SIMPLEWRITEHANDLER_H
#define PRAO_CNN_SIMPLEWRITEHANDLER_H


#include <string>
#include <utility>
#include <vector>
#include "../Celestial/Timestamp.h"
#include "../Time/TimeCoordinate.h"
#include "../Config/Config.h"
#include <sys/stat.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include "../Utils/Utils.h"

class SimpleWriteHandler {
private:
    std::string outputPath;
    std::vector<Timestamp> calculatedData;
    TimeCoordinate timeCoordinate;

    std::string getDirPathFromTm(tm *dateTime);

    bool isWriteRawData = false;


public:
    SimpleWriteHandler(const Config &config, std::vector<Timestamp> calculatedData, TimeCoordinate coordinate) {
        this->outputPath = config.getOutputPath();
        this->calculatedData = std::move(calculatedData);
        this->timeCoordinate = std::move(coordinate);
        this->isWriteRawData = config.isWriteRawData();
    }

    void write();

    void writeToFile(const std::string &filepath, int ray_num, const std::vector<float> &fourierResult);
};


#endif //PRAO_CNN_SIMPLEWRITEHANDLER_H
