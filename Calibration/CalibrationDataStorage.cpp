

#include "CalibrationDataStorage.h"


int CalibrationDataStorage::add_items_from_stream(istream &stream) {
    int count = 0;
    string line1, line2;


    while (!stream.eof()){
        getline(stream, line1, '\n');
        if (line1 == "\r")
            getline(stream, line1, '\n');

        getline(stream, line2, '\n');
        if (line2 == "\r")
            getline(stream, line2, '\n');

        if (line1.empty())
            break;

        st.insert(new CalibrationData(line1, line2));
        ++count;
    }
    return count;
}

CalibrationDataStorage::CalibrationDataStorage(string &data) {
    add_items(data);
}

int CalibrationDataStorage::add_items(string &data) {
    stringstream stream(data);
    return add_items_from_stream(stream);
}


int CalibrationDataStorage::add_items_from_file(string &path) {
    ifstream stream(path, ios::binary);
    if (!stream.good())
        throw logic_error(path + " file not exists");
    return add_items_from_stream(stream);
}

CalibrationData* CalibrationDataStorage::getCalibrationData_left_by_UTC(int year, int mon, int day, int hour, int min, int sec) {
    tm time = { sec, min, hour, day, mon, year, 0, 0, 0 };
    return getCalibrationData_left_by_time(to_MJD(time));
}

CalibrationData* CalibrationDataStorage::getCalibrationData_right_by_UTC(int year, int mon, int day, int hour, int min, int sec) {
    tm time = { sec, min, hour, day, mon, year, 0, 0, 0 };
    return getCalibrationData_right_by_time(to_MJD(time));
}

CalibrationData* CalibrationDataStorage::getCalibrationData_left_by_time(double time_MJD) {
    CalibrationData to_search(time_MJD);
    auto next = st.upper_bound(&to_search);
    if (next == st.begin())
        throw logic_error("too early or set is empty");
    return *(--next);
}

CalibrationData* CalibrationDataStorage::getCalibrationData_right_by_time(double time_MJD) {
    CalibrationData to_search(time_MJD);
    auto next = st.upper_bound(&to_search);
    if (next == st.end())
        throw logic_error("out of range: too late");
    return *next;
}

void CalibrationDataStorage::print() {
    for (auto i : st)
        i->print_date();
}
