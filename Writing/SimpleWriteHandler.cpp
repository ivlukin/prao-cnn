//
// Created by sorrow on 07.04.2021.
//


#include "SimpleWriteHandler.h"

void SimpleWriteHandler::write() {
    time_t beginDateTime = to_SunTime(timeCoordinate.getBeginDateTime());
    tm *beginDateTimeTm = localtime(&beginDateTime);
    if (beginDateTimeTm->tm_year < 200) {
        beginDateTimeTm->tm_year += 1900;
        beginDateTimeTm->tm_mon += 1;
    }
    std::string dirPath = getDirPathFromTm(beginDateTimeTm);
    std::string totalDirPath = outputPath + getSystemSeparator() + dirPath;

#ifdef _WIN32
    CreateDirectory(totalDirPath.c_str(), nullptr);
#else
    mkdir(totalDirPath.c_str(), 0777);
#endif
    for (Timestamp timestamp: calculatedData) {
        std::string subDirPath = totalDirPath + getSystemSeparator() +
                                 getDirPathFromTm(timestamp.getTimestamp());
        for (const auto &raySummaryEntry: timestamp.getRayMap()) {
            int ray_num = raySummaryEntry.first + 1;
            Ray thisRay = raySummaryEntry.second;
            std::vector<float> bandSummaryForThisRay = thisRay.getBandSummary();
            std::string filePath = subDirPath + getSystemSeparator() + std::to_string(ray_num) + ".fou";
            writeToFile(filePath, ray_num, bandSummaryForThisRay);
        }
    }
}

std::string SimpleWriteHandler::getDirPathFromTm(tm *dateTime) {
    std::string path;

    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') << dateTime->tm_mday;
    std::string s = ss.str();
    path += s;

    ss = std::stringstream();
    ss << std::setw(2) << std::setfill('0') << dateTime->tm_mon;
    s = ss.str();
    path += s;

    path += std::to_string(dateTime->tm_year).substr(2);
    path += "_";

    ss = std::stringstream();
    ss << std::setw(2) << std::setfill('0') << dateTime->tm_hour;
    s = ss.str();
    path += s;
    path += "_";

    ss = std::stringstream();
    ss << std::setw(2) << std::setfill('0') << dateTime->tm_min;
    s = ss.str();
    path += s;
    path += "_";

    ss = std::stringstream();
    ss << std::setw(2) << std::setfill('0') << dateTime->tm_sec;
    s = ss.str();
    path += s;

    return path;
}

std::string SimpleWriteHandler::getSystemSeparator() {
#ifdef _WIN32
    return "\\";
#else
    return "/";
#endif
}

void
SimpleWriteHandler::writeToFile(const std::string &filepath, int ray_num, const std::vector<float> &fourierResult) {

    std::ofstream out(filepath);
    double tresolution = fourierResult.size() == (2048 / 2 + 1) ? 100 : 12.5;
    double _localTime = to_SunTime(timeCoordinate.getBeginDateTime());

    out << "numpar\t" << 4 << std::endl;
    out << "ray_number\t" << ray_num << std::endl;
    out << std::fixed << std::setprecision(4) << "msk_begin\t" << _localTime << std::endl;
    out << "time_resolution\t" << tresolution << std::endl;

    out.close();

    std::vector<float> resultAsFloat = std::vector<float>();
    resultAsFloat.reserve(fourierResult.size());
    for (double value: fourierResult)
        resultAsFloat.push_back((float) value);

    FILE *f = fopen(filepath.c_str(), "ab");
    size_t resultSize = resultAsFloat.size();
    fwrite(&resultAsFloat[0], sizeof(std::vector<float>::value_type), resultSize, f);

    fclose(f);
}
