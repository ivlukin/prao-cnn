
#include "FileHandler.h"



FileHandler::FileHandler(const std::vector<double> &timeCoordinates, const Config &config) {
    this->mode = config.getMode();
    this->range = config.getRange();
    this->fileListPath = config.getFileListPath();
    this->timeCoordinatesEpoch = timeCoordinates;
}


void FileHandler::processFilesItemsList() {
    std::vector<FilesListItem> fileItems = std::vector<FilesListItem>();
    std::vector<std::string> fileNameList = std::vector<std::string>();
    // сначала сгруппируем таймштампы по принципу имя_файла -> соответствующие таймштампы
    std::map<std::string, std::vector<tm *>> fileNameToTimestampsList = std::map<std::string, std::vector<tm *>>();
    for (tm *time: this->timeCoordinates) {
        // мы уже сделали month + 1, здесь нужно вычесть
        std::string fileNameFromDate = Utils::getFileFromDate(time->tm_year, time->tm_mon - 1, time->tm_mday, time->tm_hour,
                                                              range, mode);
        fileNameList.push_back(fileNameFromDate);
        if (fileNameToTimestampsList.find(fileNameFromDate) == fileNameToTimestampsList.end()) {
            fileNameToTimestampsList.insert(std::pair<std::string,
                    std::vector<tm *>>(fileNameFromDate, std::vector<tm *>()));
        }
        fileNameToTimestampsList[fileNameFromDate].push_back(time);
    }
    // затем найдем соответствующие файл-айтемы (они все должны быть!)
    ifstream in(this->fileListPath);
    if (!in.good())
        throw std::logic_error(fileListPath + " not found!");
    while (!in.eof()) {
        FilesListItem item;
        in >> item;
        if (std::find(fileNameList.begin(), fileNameList.end(), item.filename) != fileNameList.end()) {
            fileItems.push_back(item);
        }
    }
    in.close();

    // затем просто смапим соответствующие файлайтемы на таймштмпы
    fileItemToTimestampsMap = std::map<FilesListItem, std::vector<tm *>>();
    for (const FilesListItem &item: fileItems) {
        fileItemToTimestampsMap.insert(std::pair<FilesListItem,
                std::vector<tm *>>(item, fileNameToTimestampsList[item.filename]));
    }
}

void FileHandler::calculateRelatedFiles() {
    this->timeCoordinates = std::vector<std::tm *>();
    for (double starTime: this->timeCoordinatesEpoch) {
        // важно получить именно солнечное время, поскольку файлы хранятся именно в солнечном календаре
        time_t sunTimeAsInt = std::floor(to_SunTime(starTime));
        tm *starTimeStruct = localtime(&sunTimeAsInt);
        if (starTimeStruct->tm_year < 200) {
            starTimeStruct->tm_year += 1900;
            starTimeStruct->tm_mon += 1;
        }
        this->timeCoordinates.push_back(tmDeepCopy(starTimeStruct));
    }
    processFilesItemsList();
}

const map<FilesListItem, std::vector<tm *>> &FileHandler::getFileNameToTimestampsMap() const {
    return fileItemToTimestampsMap;
}

