
#include <cmath>
#include "DataSeeker.h"

std::vector<float> DataSeeker::seek(int ray, int band, int timeElapsedInSeconds, int size) {
    std::ifstream filestream(filename, std::ios::in | std::ios::binary);
    std::vector<float> read_data = std::vector<float>();
    long elapsedPoints = (int) (timeElapsedInSeconds / dataHeader.tresolution);
    long caret =
            getCaretPosition(dataHeader.nrays, dataHeader.nbands, ray, band, this->header_length, elapsedPoints);

    filestream.seekg(caret, std::ifstream::beg);
    char *buffer = new char[sizeof(float)];
    for (int i = 0; i < size; ++i) {
        //read data and cast it to float
        filestream.read(buffer, sizeof(float));
        float signal = ((float *) buffer)[0];
        read_data.push_back(signal);
        ++elapsedPoints;
        caret = getCaretPosition(dataHeader.nrays, dataHeader.nbands, ray, band, this->header_length, elapsedPoints);
        filestream.seekg(caret, std::ifstream::beg);
    }
    delete[] buffer;
    filestream.close();

    double beginPointMJD = dataHeader.MJD_begin + ((float) timeElapsedInSeconds / dataHeader.tresolution) / 86400;
    CalibrationData *left = calibrationData->getCalibrationData_left_by_time(beginPointMJD);
    for (int i = 0; i < read_data.size(); ++i) {
        read_data[i] = (read_data[i] - (double) left->get_zero_level()[300]) / left->get_one_kelvin()[300];
    }
    return read_data;
}

long DataSeeker::getCaretPosition(int nrays, int nbands, int ray, int band, int _header_length, int elapsedPoints) {
    long caret =
            (long) (nrays * nbands * sizeof(float) * elapsedPoints)
            + _header_length +
            (long) (ray * dataHeader.nbands * sizeof(float)) + band * sizeof(float);
    return caret;
}
