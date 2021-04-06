

#include "Config.h"
using namespace rapidjson;
Config::Config(char *fileName) {
    std::cout << "parsing config..." << std::endl;
    FILE *fp = fopen(fileName, "r"); // non-Windows use "r"
    char readBuffer[2048];
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    Document d;
    d.ParseStream(is);
    fclose(fp);

    assert(d.HasMember("startDate")); // date of observation started
    assert(d.HasMember("observationLength")); // length of observation (in days)
    assert(d.HasMember("range")); // north or south
    assert(d.HasMember("step")); // scan step (in seconds)
    assert(d.HasMember("outputPath")); // directory for storing results
    assert(d.HasMember("mode")); // mode (fast or standard)
    assert(d.HasMember("fileListPath")); // path to .txt file with files
    assert(d.HasMember("calibrationListPath")); // path to .txt file with calibration data
    assert(d.HasMember("durationStarSeconds")); // duration of batch in star seconds

    this->startDate= d["startDate"].GetString();
    this->outputPath = d["outputPath"].GetString();
    this->observationLength = d["observationLength"].GetInt();
    this->range = d["range"].GetString();
    this->step = d["step"].GetInt();
    this->mode = d["mode"].GetString();
    this->fileListPath = d["fileListPath"].GetString();
    this->calibrationListPath = d["calibrationListPath"].GetString();
    this->durationStarSeconds = d["durationStarSeconds"].GetDouble();
}

const std::string &Config::getStartDate() const {
    return startDate;
}


const std::string &Config::getRange() const {
    return range;
}

int Config::getStep() const {
    return step;
}

const std::string &Config::getOutputPath() const {
    return outputPath;
}

const std::string &Config::getMode() const {
    return mode;
}

const std::string &Config::getFileListPath() const {
    return fileListPath;
}

const std::string &Config::getCalibrationListPath() const {
    return calibrationListPath;
}

double Config::getDurationStarSeconds() const {
    return durationStarSeconds;
}

int Config::getObservationLength() const {
    return observationLength;
}
