
#ifndef PRAO_FOURIER_FOURIERHANDLER_H
#define PRAO_FOURIER_FOURIERHANDLER_H


#include <utility>
#include <vector>
#include "../Reader/FilesListItem.h"
#include "../Config/Config.h"
#include "../Reader/DataSeeker.h"
#include "../Time/TimeCoordinate.h"
#include "../Context/OpenCLContext.h"
#include "FourierTransformer.h"
#include "../Celestial/Ray.h"
#include <cmath>
#include "../Celestial/Timestamp.h"
#include <iomanip>
#include "../Utils/Utils.h"

class FourierHandler {
private:
    double duration;
    std::map<FilesListItem, std::vector<tm*>> fileItemToTimestampsMap;
    OpenCLContext context;
    std::vector<Timestamp> calculatedData;
    CalibrationDataStorage *storage;
    bool isWriteRawData = false;

public:
    FourierHandler() = default;

    FourierHandler(const Config &config, std::map<FilesListItem, std::vector<tm*>> fileItemToTimestampsMap, OpenCLContext context) {
        this->duration = config.getDurationStarSeconds();
        this->fileItemToTimestampsMap = std::move(fileItemToTimestampsMap);
        this->context = context;
        this->isWriteRawData = config.isWriteRawData();
    }
    int run();

    void setStorage(CalibrationDataStorage *storage);

    const vector<Timestamp> &getCalculatedData() const {
        return calculatedData;
    }

};


#endif //PRAO_FOURIER_FOURIERHANDLER_H
