

#include "FourierHandler.h"


int FourierHandler::run() {
    this->calculatedData = std::vector<Timestamp>();
    for (auto const &entry: this->fileItemToTimestampsMap) {
        FilesListItem item = entry.first;
        std::vector<tm *> timeStamps = entry.second;
        auto *seeker = new DataSeeker(item.filepath);
        seeker->setCalibrationData(this->storage);
        int size = floor(duration / seeker->getHeader().tresolution);
        for (tm *timestamp: timeStamps) {
            time_t epochSecondsSunTime = mktime(timestamp);
            time_t timeElapsedFromHourBegin = epochSecondsSunTime % (60 * 60);
            Timestamp skyTimestamp = Timestamp(timestamp);
            for (int _ray = 0; _ray < 48; _ray++) {
                Ray ray = Ray(_ray + 1, seeker->getHeader().nbands - 1);
                std::vector<float> complexAmplitudes = std::vector<float>((size / 2 + 1) * 2);
                std::fill(complexAmplitudes.begin(), complexAmplitudes.end(), 0);
                // суммирующий band не нужен
                for (int band = 0; band < seeker->getHeader().nbands - 1; ++band) {
                    std::vector<float> readData;
                    // проверка на перескок через файл
                    if (timeElapsedFromHourBegin + to_SunTime(this->duration) <= 3600) {
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

                    // просто суммируем по всем частотам, затем посчитаем среднее
                    for (int j = 0; j < (size / 2 + 1) * 2; ++j)
                        complexAmplitudes[j] += result[j];

                    // очень важно очистить память после перекидывания посчитанных результатов
                    delete[] result;
                }
                ray.setComplexAmplitudes(complexAmplitudes);
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