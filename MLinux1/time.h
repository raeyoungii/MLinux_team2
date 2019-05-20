#include <time.h>
#ifndef TIME_H_
#define TIME_H_

time_t ltime;
struct tm *today;

struct tagTime{
    int month;
    int day;
    int hour;
    int minute;
}dirTime;

#endif // TIME_H_
