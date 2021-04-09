//
// Created by sorrow on 09.04.2021.
//

#ifndef PRAO_CNN_UTILS_H
#define PRAO_CNN_UTILS_H


#include <string>
#ifdef _WIN32
#include "fileapi.h"
#else
#include <sys/stat.h>
#endif

class Utils {
public:
    static void createDirectory(std::string dirPath);
    static std::string getSystemSeparator();
};


#endif //PRAO_CNN_UTILS_H
