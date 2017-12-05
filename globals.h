#ifndef GLOBALS_H
#define GLOBALS_H

#include <simlib.h>
using namespace simlib3;


extern Facility TisnovToKurimStation;
extern Queue TisnovToKurimQ;

extern Facility KurimToBrnoStation;
extern Queue KurimToBrnoQ;

extern Facility BrnoEndStation;

extern Facility TisnovEndStation;
extern Facility KurimToTisnovStation;
extern Queue KurimToTisnovQ;

extern Facility BrnoToKurimStation;
extern Queue BrnoToKurimQ;

extern Stat trainStat;
extern Stat commuterLeavingStat;
extern Stat trainFullnessStatWorkDay;
extern Stat trainFullnessStatWorkNight;
extern Stat trainFullnessStatWeekendDay;
extern Stat trainFullnessStatWeekendNight;

extern Histogram TisnovToKurimQHist;

enum STATION {TISNOV, KURIMT, KURIMB, BRNO}; 

#endif