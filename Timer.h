#ifndef TIMER_H
#define TIMER_H

#include "constants.h"

enum DAYTIME{DAY, NIGHT};
enum DAYTYPE{WORKDAY, WEEKEND};

class Timer{
public:
	static DAYTIME getDayTime(double time);
	static DAYTYPE getDayType(double time);
};

#endif