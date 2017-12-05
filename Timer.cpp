#include "Timer.h"
#include <math.h>

/**
 * Vypocet denni doby podle modeloveho casu
 */
DAYTIME Timer::getDayTime(double time){
	double dayTime = fmod(time, DAY_LENGTH);

	if(dayTime >= DAY_START && dayTime <= DAY_END){
		return DAY;
	} else {
		return NIGHT;
	}
}

/**
 * Vypocet typu dne podle mdoelvoeho casu
 */
DAYTYPE Timer::getDayType(double time){
	double weekTime = fmod(time, WEEK_LENGTH);

	if(weekTime >= WORK_WEEK_START && weekTime <= WORK_WEEK_END){
		return WORKDAY;
	} else {
		return WEEKEND;
	}
}