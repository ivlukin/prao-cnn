
#include <iomanip>
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
                                  << calculateNextFileName(item.filepath) << std::endl;
                        auto *anotherSeeker = new DataSeeker(calculateNextFileName(item.filepath));
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

std::string FourierHandler::calculateNextFileName(std::string filepath) {
//system separator should be here
    std::string absolutePath = filepath.substr(0, filepath.find_last_of("//") + 1);
    std::string currentFileName = filepath.substr(filepath.find_last_of("//") + 1);
    vector<string> tokens = split(currentFileName, "_");
    string extension = tokens[3].substr(tokens[3].find_last_of('.'));
    string mode = tokens[2];
    string hour = tokens[1];
    string currentDateWithoutTime = tokens[0];
    currentDateWithoutTime += " " + hour + ":00:00";
    tm *dateTime = new tm();
    std::istringstream ss(currentDateWithoutTime);
    if (ss >> get_time(dateTime, "%d%m%y %H:%M:%S")) {
        std::put_time(dateTime, "%c");
    }
    dateTime->tm_hour += 1;
    mktime(dateTime);
    std::string result = absolutePath + getFileNameFromDate(dateTime->tm_year, dateTime->tm_mon + 1, dateTime->tm_mday,
                                                            dateTime->tm_hour, mode, extension);
    delete dateTime;
    return result;
}

std::vector<string> FourierHandler::split(const string &str, const string &delim) {
    std::vector<string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

std::string FourierHandler::getFileNameFromDate(int year, int month, int day, int hour, string range, string mode) {
    std::string path;

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << day;
    std::string s = ss.str();
    path += s;

    ss = std::stringstream();
    ss << std::setw(2) << std::setfill('0') << month;
    s = ss.str();
    path += s;

    path += std::to_string(year);
    path += "_";

    ss = std::stringstream();
    ss << std::setw(2) << std::setfill('0') << hour;
    s = ss.str();
    path += s;

    path += "_" + range + "_00" + mode;

    return path;
}
