
#include "DataHeader.h"

ifstream &operator>>(ifstream & in, DataHeader& dt){

    string tmp;

    in >> tmp >> dt.numpar;
    if (tmp != "numpar")
        throw logic_error("numpar not found");

    in >> tmp >> dt.source >> tmp >> dt.alpha >> tmp >> dt.delta >> tmp >> dt.fcentral >> tmp >> dt.wb_total;


    tm tm_local = {};
    string datetime_local, datetime_UTC;
    in >> tmp >> datetime_local >> tmp >> datetime_UTC;
    sscanf(datetime_local.c_str(), "%d.%d.%d", &tm_local.tm_mday, &tm_local.tm_mon, &tm_local.tm_year);
    sscanf(datetime_UTC.c_str(), "%d.%d.%d", &dt.begin_datetime.tm_mday, &dt.begin_datetime.tm_mon, &dt.begin_datetime.tm_year);

    in >> tmp >> datetime_local >> tmp >> datetime_UTC;
    sscanf(datetime_local.c_str(), "%d:%d:%d", &tm_local.tm_hour, &tm_local.tm_min, &tm_local.tm_sec);
    sscanf(datetime_UTC.c_str(), "%d:%d:%d", &dt.begin_datetime.tm_hour, &dt.begin_datetime.tm_min, &dt.begin_datetime.tm_sec);
    dt.MJD_begin = to_MJD(dt.begin_datetime);
    tm_SubDefault(tm_local);

    int diff_local_and_UTC_in_seconds = difftime(mktime(&tm_local), mktime(&dt.begin_datetime));


    in >> tmp >> datetime_local;
    sscanf(datetime_local.c_str(), "%d.%d.%d", &dt.end_datetime.tm_mday, &dt.end_datetime.tm_mon, &dt.end_datetime.tm_year);
    in >> tmp >> datetime_local;
    sscanf(datetime_local.c_str(), "%d:%d:%d", &dt.end_datetime.tm_hour, &dt.end_datetime.tm_min, &dt.end_datetime.tm_sec);
    dt.end_datetime.tm_sec -= diff_local_and_UTC_in_seconds;
    dt.MJD_end = to_MJD(dt.end_datetime);

    in >> tmp;
    getline(in, tmp, '\n');
    stringstream stream(tmp);
    while (!stream.eof()){
        int curr;
        stream >> curr;
        dt.modulus.push_back(curr);
    }

    in >> tmp >> dt.tresolution >> tmp >> dt.npoints >> tmp >> dt.nbands;
    if (100 - dt.tresolution < 0.1)
        dt.tresolution = 100;
    dt.tresolution /= 1000;

    in >> tmp;
    getline(in, tmp, '\n');
    stream = stringstream(tmp);
    while (!stream.eof()){
        float curr;
        stream >> curr;
        dt.wbands.push_back(curr);
    }

    in >> tmp;
    getline(in, tmp, '\n');
    stream = stringstream(tmp);
    while (!stream.eof()){
        float curr;
        stream >> curr;
        dt.fbands.push_back(curr);
    }

    return in;
}