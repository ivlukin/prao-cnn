
#ifndef PRAO_CLASSIFIER_TIME_H
#define PRAO_CLASSIFIER_TIME_H


#include <ctime>

/// subs from tm 1900 years and makes mon in range [0-11]
int tm_SubDefault(tm &time);

/// adds to tm 1900 years and makes mon in range [1-12]
int tm_AddDefault(tm &time);


/// subs from tm 4 hours and makes it in default view
int tm_toUTC(tm &time, int seconds = 60 * 60 * 4);

double to_MJD(tm &time);

double to_starTime(double sunSeconds);

double to_SunTime(double starSeconds);

tm* tmDeepCopy(tm* self);



#endif //PRAO_CLASSIFIER_TIME_H
