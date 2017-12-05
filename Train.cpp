#include "Train.h"
#include <simlib.h>
#include "Timer.h"
#include "globals.h"
#include <iostream>
using namespace simlib3;

/**
 * Vytvoreni vlaku obema smery. Urcen denni dobou. Definice casoveho rozlozeni v constants.h
 */
void TrainGenerator::Behavior(){
	(new Train(true))->Activate();
	(new Train(false))->Activate();
	switch(Timer::getDayType(Time)){
		case WORKDAY:
			switch(Timer::getDayTime(Time)){
				case DAY:
					Activate(Time + TRAIN_DAY_TIME_DISTRIBUTION);
					break;
				case NIGHT:
					Activate(Time + TRAIN_NIGHT_TIME_DISTRIBUTION);
					break;
			}
			break;
		case WEEKEND:
			switch(Timer::getDayTime(Time)){
				case DAY:
					Activate(Time + TRAIN_DAY_WEEKEND_TIME_DISTRIBUTION);
					break;
				case NIGHT:
					Activate(Time + TRAIN_NIGHT_WEEKEND_TIME_DISTRIBUTION);
					break;
			}
			break;
		default: break;
	}
}

Train::Train(bool startsInBrno){
	this->startsInBrno = startsInBrno;
}

/**
 * Destruktor, uklada statistiky zpozdeni
 */
Train::~Train(){
	trainStat(this->delay);
}

/**
 * Funkce podle smeru cesty.
 */
void Train::Behavior(){
	if(startsInBrno)
		BrnoToTisnov();
	else
		TisnovToBrno();
}

/**
 * Pridani cestujiciho do vlaku. 
 * @return Pokud je vlak plny, vraci false, jinak true.
 */
bool Train::joinTrain(Commuter* person){
	if(people.Length() == this->capacity)
		return false;
	people.Insert(person);
	return true;
}

/**
 * Generovani zpozdeni. Je pridano do celkove doby cesty vlaku.
 * @return zpozdeni vlaku v minutach.
 */
double Train::generateDelay(){
	const double MINOR_THRESHOLD = 0.15; //%
	const double MAJOR_THRESHOLD = MINOR_THRESHOLD + 0.05; //%
	const double BREAKDOWN_THRESHOLD = MAJOR_THRESHOLD + 0.01; //%
	double chance = Random();

	if(chance < MINOR_THRESHOLD)
		return Exponential(5);
	if(chance < MAJOR_THRESHOLD)
		return Exponential(15);
	if(chance < BREAKDOWN_THRESHOLD)
		return Uniform(30, 90);
	return 0;
}

/**
 * Trasa z Tisnova do Brna.
 */
void Train::TisnovToBrno(){
	Seize(TisnovToKurimStation);
	prichoziTisnov(TisnovToKurimQ.Length());
	activateAllInQueue(&TisnovToKurimQ); //nastup cestujicich
	Wait(1);
	Release(TisnovToKurimStation);
	logFullness();

	double delay = this->generateDelay();
	this->delay += delay;
	
	Wait(TISNOV_KURIM_TIME + delay); //cesta do Kurima

	Seize(KurimToBrnoStation);
	activateAllInQueue(&KurimToBrnoQ); //nastup cestujicich
	activateAllInQueue(&people); //vystup cestujicich
	Wait(1);
	Release(KurimToBrnoStation);
	logFullness();

	delay = this->generateDelay();
	this->delay += delay;

	Wait(KURIM_BRNO_TIME + delay); //cesta do Brna

	Seize(BrnoEndStation);
	activateAllInQueue(&people); //vystup cestujicich
	Wait(1);
	Release(BrnoEndStation);
}

/**
 * Trasa z Brna do Tisnova
 */
void Train::BrnoToTisnov(){
	Seize(BrnoToKurimStation);
	activateAllInQueue(&BrnoToKurimQ); //nastup cestujicich
	Wait(1);
	Release(BrnoToKurimStation);
	logFullness();

	double delay = generateDelay();
	this->delay += delay;

	Wait(KURIM_BRNO_TIME + delay);

	Seize(KurimToTisnovStation);
	activateAllInQueue(&KurimToTisnovQ); //nastup cestujicich
	activateAllInQueue(&people); //vystup cestujicich
	Wait(1);
	Release(KurimToTisnovStation);
	logFullness();

	delay = generateDelay();
	this->delay += delay;

	Wait(TISNOV_KURIM_TIME + delay);

	Seize(TisnovEndStation);
	activateAllInQueue(&people); //vystup cestujicich
	Wait(1);
	Release(TisnovEndStation);
}

/**
 * Aktivace vsech cestujicich ve vlaku.
 */
void Train::activateAllInQueue(Queue* q){
	while(!q->empty()){
		Process* tmp = (Process*)q->GetFirst();
		tmp->Activate();
	}
}

/**
 * Ulozeni hodnot zaplneni vlaku do objektu Stat.
 */
void Train::logFullness(){
	switch(Timer::getDayType(Time)){
		case WORKDAY:
			switch(Timer::getDayTime(Time)){
				case DAY:
					trainFullnessStatWorkDay(this->people.Length());
					break;
				case NIGHT:
					trainFullnessStatWorkNight(this->people.Length());
					break;
			}
			break;
		case WEEKEND:
			switch(Timer::getDayTime(Time)){
				case DAY:
					trainFullnessStatWeekendDay(this->people.Length());
					break;
				case NIGHT:
					trainFullnessStatWeekendNight(this->people.Length());
					break;
			}
			break;
		default: break;
	}
}