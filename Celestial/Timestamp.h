
#ifndef PRAO_FOURIER_TIMESTAMP_H
#define PRAO_FOURIER_TIMESTAMP_H


#include <map>
#include "Ray.h"

class Timestamp {
private:
    std::map<int, Ray> rayMap;
    tm* timestamp;
public:
    Timestamp() = default;
    explicit Timestamp(tm* timestamp) {
        setTimestamp(timestamp);
        rayMap = std::map<int, Ray>();
    }

    std::map<int, Ray> &getRayMap() {
        return rayMap;
    }

    void setRayMap(const std::map<int, Ray> &_rayMap) {
        this->rayMap = _rayMap;
    }

    tm *getTimestamp() const {
        return timestamp;
    }

    void setTimestamp(tm *_timestamp) {
        this->timestamp = _timestamp;
    }
};


#endif //PRAO_FOURIER_TIMESTAMP_H
