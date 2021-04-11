
#include "DataSeeker.h"

std::vector<float> DataSeeker::seek(int ray, int band, int timeElapsedInSeconds, int size) {
    std::ifstream filestream(filename, std::ios::in | std::ios::binary);
    std::vector<float> read_data = std::vector<float>();
    long long elapsedPoints = (int) (timeElapsedInSeconds / dataHeader.tresolution);
    long long caret =
            (long) (dataHeader.nrays * dataHeader.nbands * sizeof(float) * elapsedPoints)
            + this->header_length +
            (long) (ray * dataHeader.nbands * sizeof(float)) + band * sizeof(float);

    filestream.seekg(caret, std::ifstream::beg);
    char *buffer = new char[sizeof(float)];
    for (int i = 0; i < size; ++i) {
        //read data and cast it to float
        filestream.read(buffer, sizeof(float));
        float signal = ((float *) buffer)[0];
        read_data.push_back(signal);
        filestream.seekg(dataHeader.nrays * dataHeader.nbands * sizeof(float), std::ifstream::cur);
    }
    count_point_position = filestream.tellg();
    delete[] buffer;
    filestream.close();
    data = std::move(read_data);
    read_data.clear();
    calibrateArrayPoints(ray * band);
    return data;
}

void DataSeeker::updateCalibrationData() {

    double currentPointMJD = dataHeader.MJD_begin + (count_point_position * dataHeader.tresolution) / 86400;
    CalibrationData *left = calibrationData->getCalibrationData_left_by_time(currentPointMJD);
    CalibrationData *right = calibrationData->getCalibrationData_right_by_time(currentPointMJD);

    realloc(calibration_on_k, left->get_one_kelvin());
    realloc(on_k_step, right->get_one_kelvin());
    realloc(calibration_zr, left->get_zero_level());
    realloc(zr_step, right->get_zero_level());

    int i = floats_per_point;
    int n = ((right->get_MJD() - left->get_MJD()) * 24 * 60 * 60 + dataHeader.tresolution - 0.000000001) /
            dataHeader.tresolution;
    while (--i >= 0) {
        on_k_step[i] = (on_k_step[i] - calibration_on_k[i]) / n;
        zr_step[i] = (zr_step[i] - calibration_zr[i]) / n;
    }
}

void DataSeeker::realloc(double *&base, double const *from) {
    delete base;
    base = new double[floats_per_point];
    memcpy(base, from, sizeof(double) * floats_per_point);
}

void DataSeeker::calibrateArrayPoints(int coordinate) {
    for (int i = 0; i < data.size(); ++i)
        data[i] = (data[i] - calibration_zr[coordinate]) / calibration_on_k[coordinate];
}
