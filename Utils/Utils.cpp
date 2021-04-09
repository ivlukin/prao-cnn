//
// Created by sorrow on 09.04.2021.
//

#include "Utils.h"

void Utils::createDirectory(std::string dirPath) {

#ifdef _WIN32
        CreateDirectory(dirPath.c_str(), nullptr);
#else
        mkdir(dirPath.c_str(), 0777);
#endif

}

std::string Utils::getSystemSeparator() {

#ifdef _WIN32
        return "\\";
#else
        return "/";
#endif

}
