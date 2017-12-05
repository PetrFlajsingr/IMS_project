#include "globals.h"
#include "constants.h"

/**
 * Soubor obsahujici definice globalnich promennych.
 */
using namespace simlib3;
Facility TisnovToKurimStation("Tisnov to Kurim");
Queue TisnovToKurimQ("Tisnov to Kurim");

Facility KurimToBrnoStation("Kurim to Brno");
Queue KurimToBrnoQ("Kurim to Brno");

Facility BrnoEndStation("Brno end");

Facility TisnovEndStation("Tisnov end");

Facility KurimToTisnovStation("Kurim to Tisnov");
Queue KurimToTisnovQ("Kurim to Tisnov");

Facility BrnoToKurimStation("Brno to Kurim");
Queue BrnoToKurimQ("Brno to Kurim");

Stat trainStat("Zpozdeni");

Stat commuterLeavingStat("Dlouhe cekani - odchod");

Stat trainFullnessStatWorkDay("Zaplneni vlaku - pracovni den");
Stat trainFullnessStatWorkNight("Zaplneni vlaku - pracovni noc");
Stat trainFullnessStatWeekendDay("Zaplneni vlaku - vikend den");
Stat trainFullnessStatWeekendNight("Zaplneni vlaku - vikend noc");

Stat incomingTisnov("Prichozi Tisnov");
Stat incomingKurimTisnov("Prichozi Kurim->Tisnov");
Stat incomingKurimBrno("Prichozi Kurim->Brno");
Stat incomingBrno("Prichozi Brno");

Histogram peopleLeavingHist("Odchozi z nastupiste", 0, DAY_LENGTH, SIMULATION_TIME_IN_DAYS);

Histogram TisnovToKurimQHist("Lidi v Tisnove", 0, DAY_LENGTH, SIMULATION_TIME_IN_DAYS);