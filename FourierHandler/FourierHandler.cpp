
#include "FourierHandler.h"


int FourierHandler::run() {
    this->calculatedData = std::vector<Timestamp>();
    for (auto const &entry: this->fileItemToTimestampsMap) {
        FilesListItem item = entry.first;
        std::vector<tm *> timeStamps = entry.second;
        DataSeeker *seeker = new DataSeeker(item.filepath);
        seeker->setCalibrationData(this->storage);
        int size = item.nbands == 33 ? 2048 * 8 : 2048; // сейчас захардкожено, вообще надо из duration считать
        for (tm *timestamp: timeStamps) {
            time_t epochSecondsStarTime = mktime(timestamp);
            time_t epochSecondsSunTime = to_SunTime(epochSecondsStarTime);
            time_t timeElapsedFromHourBegin = epochSecondsSunTime % (60 * 60);
            if (timeElapsedFromHourBegin + this->duration > 3600) {
                int tail = (int) (timeElapsedFromHourBegin + this->duration) - 3600 + 1;
                timeElapsedFromHourBegin -= tail;
            }
            Timestamp skyTimestamp = Timestamp(timestamp);
            for (int _ray = 0; _ray < 48; _ray++) {
                Ray ray = Ray(_ray + 1);
                std::map<int, std::vector<float>> bandMap;
                for (int band = 0; band < item.nbands; ++band) {
                    std::vector<float> readData = seeker->seek(_ray, band, timeElapsedFromHourBegin, size);
                    FourierTransformer fourierTransformer = FourierTransformer(context, readData.data(),
                                                                               readData.size());
                    fourierTransformer.transform();
                    float *result = fourierTransformer.getResult();
                    fourierTransformer.releaseResources();
                    std::vector<float> modulus;
                    for (int j = 0; j < (size / 2 + 1) * 2 - 1; j += 2) {
                        float real = result[j];
                        float imaginary = result[j + 1];
                        float modul = std::sqrt(real * real + imaginary * imaginary);
                        modulus.push_back(modul);
                    }
                    bandMap[band] = modulus;
                    // очень важно очистить память после перекидывания посчитанных результатов
                    delete[] result;
                }
                ray.setBandMap(bandMap);
                std::pair<int, Ray> to_insert = std::pair<int, Ray>(_ray + 1, ray);
                skyTimestamp.getRayMap()[_ray] = ray;
            }
            calculatedData.push_back(skyTimestamp);
        }
    }
    return 0;
}


void FourierHandler::setStorage(CalibrationDataStorage *storage) {
    FourierHandler::storage = storage;
}
