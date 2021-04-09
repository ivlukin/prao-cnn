

#include "FourierHandler.h"



int FourierHandler::run() {
    this->calculatedData = std::vector<Timestamp>();
    for (auto const &entry: this->fileItemToTimestampsMap) {
        FilesListItem item = entry.first;
        std::vector<tm *> timeStamps = entry.second;
        auto *seeker = new DataSeeker(item.filepath);
        std::cout << "reading from file: " << item.filepath << std::endl;
        seeker->setCalibrationData(this->storage);
        int size = item.nbands == 33 ? 2048 * 8 : 2048; // сейчас захардкожено, вообще надо из duration считать
        for (tm *timestamp: timeStamps) {
            time_t epochSecondsStarTime = mktime(timestamp);
            time_t epochSecondsSunTime = to_SunTime(epochSecondsStarTime);
            time_t timeElapsedFromHourBegin = epochSecondsSunTime % (60 * 60);
            Timestamp skyTimestamp = Timestamp(timestamp);
            for (int _ray = 0; _ray < 48; _ray++) {
                Ray ray = Ray(_ray + 1);
                std::map<int, std::vector<float>> bandMap;
                for (int band = 0; band < item.nbands; ++band) {
                    std::vector<float> readData;
                    // проверка на перескок через файл
                    if (timeElapsedFromHourBegin + this->duration <= 3600) {
                        // здесь перескока нет, все ок
                        readData = seeker->seek(_ray, band, timeElapsedFromHourBegin, size);
                    } else {
                        // здесь есть перескок
                        int tail = (int) (timeElapsedFromHourBegin + this->duration) - 3600 + 1;
                        timeElapsedFromHourBegin -= tail;
                        // сначала считываем со смещением влево
                        readData = seeker->seek(_ray, band, timeElapsedFromHourBegin, size);
                        // затем удаляем лишнее слева
                        int removeCount = (int) ((float) tail / item.tresolution);
                        std::vector<decltype(readData)::value_type>(readData.begin() + removeCount,
                                                                    readData.end()).swap(
                                readData);
                        // считываем из нового файла
                        std::cout << "reading " << tail << " seconds from another file: "
                                  << Utils::getNextFileFromFileName(item.filepath) << std::endl;
                        auto *anotherSeeker = new DataSeeker(Utils::getNextFileFromFileName(item.filepath));
                        anotherSeeker->setCalibrationData(this->storage);
                        // очень важно, что timeElapsedInSeconds = 0
                        std::vector<float> dataFromAnotherFile = seeker->seek(_ray, band, 0, size);
                        // нам не нужно все size точек, а только tail / tresolution, добавляем их справа
                        for (int indexOfAnotherData = 0; indexOfAnotherData < removeCount; ++indexOfAnotherData) {
                            readData.push_back(dataFromAnotherFile[indexOfAnotherData]);
                        }
                        delete anotherSeeker;
                    }
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
        delete seeker;
    }
    return 0;
}


void FourierHandler::setStorage(CalibrationDataStorage *storage) {
    FourierHandler::storage = storage;
}