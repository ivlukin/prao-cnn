
#ifndef PRAO_COMPRESSER_FILESLISTITEM_H
#define PRAO_COMPRESSER_FILESLISTITEM_H

#include <ctime>
#include <string>
#include <regex>

#include "../Time/TimeUtils.h"


using namespace std;

struct FilesListItem{
    tm time_UTC = {};
    string filename = string();
    string filepath = string();
    int nbands = 0, npoints = 0;
    double tresolution = 0, star_time_start = 0, star_time_end = 0, time_JD = 0;


    friend istream &operator>>(istream & in, FilesListItem& dt);

    bool operator < (const FilesListItem& other) const {
        return time_JD < other.time_JD;
    }
};


#endif //PRAO_COMPRESSER_FILESLISTITEM_H
