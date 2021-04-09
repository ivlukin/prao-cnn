
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

    /**
     * Поскольку возможна ситуация, когда нужный отрезок лежит на границе файла
     * и часть данных нужно считать с одного файла, а часть с - другого
     * нужен метод, высчитывающий имя следующего файла
     * (вообще то это костыль)
     * @param filepath - имя первого файла
     * @return - путь к следующему файлу
     */
    std::string calculateNextFileName(std::string filepath);
    std::vector<string> split(const string &str, const string &delim);
    std::string getFileNameFromDate(int year, int month, int day, int hour, string range, string mode);

public:
    FourierHandler() = default;

    FourierHandler(std::map<FilesListItem, std::vector<tm*>> fileItemToTimestampsMap, OpenCLContext context, double duration) {
        this->duration = duration;
        this->fileItemToTimestampsMap = std::move(fileItemToTimestampsMap);
        this->context = context;
    }
    int run();

    void setStorage(CalibrationDataStorage *storage);

    const vector<Timestamp> &getCalculatedData() const {
        return calculatedData;
    }

};


#endif //PRAO_FOURIER_FOURIERHANDLER_H
