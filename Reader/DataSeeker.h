

#ifndef PRAO_CLASSIFIER_DATASEEKER_H
#define PRAO_CLASSIFIER_DATASEEKER_H


#include <fstream>
#include "DataHeader.h"
#include "../Calibration/CalibrationDataStorage.h"
#include <utility>


class DataSeeker {
    //constructors and destructors
public:
    DataSeeker() = default;

    ~DataSeeker() {
        if (in.is_open())
            in.close();
    }

    explicit DataSeeker(const std::string &filename) {
        this->filename = filename;
        in = std::ifstream(filename, std::ios::in | std::ios::binary);
        if (!in.good()) {
            throw std::logic_error("error opening data file: " + filename);
        }
        in >> dataHeader;
        dataHeader.nbands++;
        floats_per_point = dataHeader.nbands * 48;
        header_length = in.tellg();
        in.close();
    }
    //main methods
public:
    std::vector<float> seek(int ray, int band, int timeElapsedInSeconds, int size);

    const DataHeader &getHeader() const {
        return dataHeader;
    }

    void setHeader(const DataHeader &header) {
        DataSeeker::dataHeader = header;
    }

    void setCalibrationData(CalibrationDataStorage *_calibrationData) {
        DataSeeker::calibrationData = _calibrationData;
        updateCalibrationData();
    }

private:
    std::ifstream in;
    int header_length{};
    DataHeader dataHeader{};
    std::string filename;
    CalibrationDataStorage* calibrationData;
    int count_point_position {};
    int floats_per_point {};
    double* calibration_on_k = nullptr;
    double* on_k_step = nullptr;
    double* calibration_zr = nullptr;
    double* zr_step = nullptr;
    std::vector<float> data;

private:
    void updateCalibrationData();


    void realloc(double *&base, const double *from);


    void calibrateArrayPoints(int coordinate);
};


#endif //PRAO_CLASSIFIER_DATASEEKER_H
