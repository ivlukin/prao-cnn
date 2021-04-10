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

std::string Utils::getFileFromDate(tm *dt, std::string mode, std::string extension) {
    tm* tmCopy = tmDeepCopy(dt);
    tmCopy->tm_hour += 1;
    mktime(tmCopy);
    std::string result = calculateFileName(tmCopy->tm_year, tmCopy->tm_mon + 1, tmCopy->tm_mday, tmCopy->tm_hour, mode, extension);
    delete tmCopy;
    return result;
}

std::string Utils::getFileFromDate(int year, int month, int day, int hour, std::string mode, std::string extension) {
    // all time coordinates will be stored epoch time (and timezone doesn't matter)
    int _year = year;
    if (year < 200) {
        year += 1900;
    }
    tm *dt = new tm();
    dt->tm_year = _year;
    dt->tm_mon = month;
    dt->tm_mday = day;
    dt->tm_hour = hour;
    mktime(dt);
    std::string result = getFileFromDate(dt, mode, extension);
    delete dt;
    return result;
}

std::string Utils::getNextFileFromFileName(std::string fileName) {
    std::string absolutePath = fileName.substr(0, fileName.find_last_of(getSystemSeparator()) + 1);
    std::string currentFileName = fileName.substr(fileName.find_last_of(getSystemSeparator()) + 1);
    std::vector<std::string> tokens = split(currentFileName, "_");
    std::string extension = tokens[3].substr(tokens[3].find_last_of('.'));
    std::string mode = tokens[2];
    std::string hour = tokens[1];
    std::string currentDateWithoutTime = tokens[0];
    currentDateWithoutTime += " " + hour + ":00:00";
    tm *dateTime = new tm();
    std::istringstream ss(currentDateWithoutTime);
    if (ss >> std::get_time(dateTime, "%d%m%y %H:%M:%S")) {
        std::put_time(dateTime, "%c");
    }
    dateTime->tm_hour += 1;
    mktime(dateTime);
    std::string result = absolutePath + calculateFileName(dateTime->tm_year, dateTime->tm_mon + 1, dateTime->tm_mday,
                                                          dateTime->tm_hour, mode, extension);
    delete dateTime;
    return result;
}

std::string Utils::calculateFileName(int year, int month, int day, int hour, std::string mode, std::string extension) {
    std::string path;

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << day;
    std::string s = ss.str();
    path += s;

    ss = std::stringstream();
    ss << std::setw(2) << std::setfill('0') << month;
    s = ss.str();
    path += s;

    if (year < 200)
        year += 1900;
    path += std::to_string(year).substr(2);
    path += "_";

    ss = std::stringstream();
    ss << std::setw(2) << std::setfill('0') << hour;
    s = ss.str();
    path += s;

    path += "_" + mode + "_00" + extension;

    return path;
}

std::vector<std::string> Utils::split(const std::string &str, const std::string &delim) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos - prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

