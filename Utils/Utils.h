//
// Created by sorrow on 09.04.2021.
//

#ifndef PRAO_CNN_UTILS_H
#define PRAO_CNN_UTILS_H


#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include "../Time/TimeUtils.h"

#ifdef _WIN32
#include "fileapi.h"
#else

#include <sys/stat.h>

#endif

class Utils {
public:
    /**
     * Создает директорию
     * Реализация отличается в зависимости от ОС
     * @param dirPath - путь к директории (наличие директорий выше не проверяется)
     */
    static void createDirectory(std::string dirPath);

    /**
     * Сепаратор путей в разных ОС
     * @return
     */
    static std::string getSystemSeparator();

    /**
     * возвращает имя файла в зависимости от времени
     * основная суть в том, что в имени файла пишется ОКОНЧАНИЕ времени наблюдений
     * а значит нужно добавлять один час, т.е. считать даты
     * @param dt
     * @return - имя файла, соответствующего дате и часу
     */
    static std::string getFileFromDate(tm *dt, std::string mode, std::string extension);

    static std::string getFileFromDate(int year, int month, int day, int hour, std::string mode, std::string extension);

    static std::string getNextFileFromFileName(std::string fileName);

    static std::vector<std::string> split(const std::string &str, const std::string &delim);

private:
    static std::string
    calculateFileName(int year, int month, int day, int hour, std::string mode, std::string extension);
};


#endif //PRAO_CNN_UTILS_H
