
#include "Train.h"
#include "globals.h"
#include <iostream>

using namespace simlib3;

/**
 * Casovac pro odchod z nastupiste (dlouh cekani)
 */
Timeout::Timeout(Commuter* person, double dt): Id(person), dt(dt){
	Activate(Time + dt);
}

void Timeout::Behavior(){
	if(Random() < PROB_TO_LEAVE) {
		peopleLeavingHist(Time);
		Id->Cancel();
		Cancel();
	} else {
		dt /= 2;
		Activate(Time + this->dt);
	}
}

CommuterGenerator::CommuterGenerator(STATION from){
	this->from = from;
}

/**
 * Generovani cestujicich podle denni doby.
 */
void CommuterGenerator::Behavior(){
	(new Commuter(this->from))->Activate();
	Activate(Time + Exponential(nextProb()));
}

double CommuterGenerator::nextProb(){
	switch(Timer::getDayType(Time)){
		case WORKDAY:
			switch(Timer::getDayTime(Time)){
				case DAY:
					if(this->from == KURIMT || this->from==KURIMB)
						return COMMUTER_KURIM_WORK_DAY_PROB;
					else
						return COMMUTER_WORK_DAY_PROB;
					break;
				case NIGHT:
					if(this->from == KURIMT || this->from==KURIMB)
						return COMMUTER_KURIM_WORK_NIGHT_PROB;
					else
						return COMMUTER_WORK_NIGHT_PROB;
					break;
			}
			break;
		case WEEKEND:
			switch(Timer::getDayTime(Time)){
				case DAY:
					if(this->from == KURIMT || this->from==KURIMB)
						return COMMUTER_KURIM_WEEKEND_DAY_PROB;
					else
						return COMMUTER_WEEKEND_DAY_PROB;
					break;
				case NIGHT:
					if(this->from == KURIMT || this->from==KURIMB)
						return COMMUTER_KURIM_WEEKEND_NIGHT_PROB;
					else
						return COMMUTER_WEEKEND_NIGHT_PROB;
					break;
			}
			break;
		default: break;
	}
}

Commuter::Commuter(STATION from){
	this->from = from;
}

/**
 * Pravdepodobnost vystupu v Kurimi
 */
bool Commuter::leaveInKurim(){
	if(OUT_IN_KURIM_PROB <= Random()){
		return true;
	}
	return false;
}

double Commuter::getTimeoutValue(){
	switch(Timer::getDayType(Time)){
		case WORKDAY:
			switch(Timer::getDayTime(Time)){
				case DAY:
					return WORK_DAY_WAITING;
					break;
				case NIGHT:
					return WORK_NIGHT_WAITING;
					break;
			}
			break;
		case WEEKEND:
			switch(Timer::getDayTime(Time)){
				case DAY:
					return WEEKEND_DAY_WAITING;
					break;
				case NIGHT:
					return WEEKEND_NIGHT_WAITING;
					break;
			}
			break;
		default: break;
	}
}
/**
 * Cesta cestujiciho. Odviji se podle nastupni stanice.
 */
void Commuter::Behavior(){
	Timeout* t = new Timeout(this, getTimeoutValue());
	switch(this->from){
		case TISNOV: 
			TisnovToKurimQHist(Time);
			TisnovToKurimQ.Insert(this);
			wait1:
			Passivate();
			if(!((Train*)(TisnovToKurimStation.in))->joinTrain(this)){
				goto wait1;
			}
			t->Cancel();
			Passivate();
			if(!leaveInKurim())
				((Train*)(KurimToBrnoStation.in))->joinTrain(this);
			break;
		case KURIMT: 
			KurimToTisnovQ.Insert(this);
			wait2:
			Passivate();
			if(!((Train*)(KurimToTisnovStation.in))->joinTrain(this)){
				goto wait2;
			}
			t->Cancel();
			Passivate();
			break;
		case KURIMB: 
			KurimToBrnoQ.Insert(this);
			wait3:
			Passivate();
			if(!((Train*)(KurimToBrnoStation.in))->joinTrain(this)){
				goto wait3;
			}
			t->Cancel();
			Passivate();
			break;
		case BRNO: 
			BrnoToKurimQ.Insert(this);
			wait4:
			Passivate();
			if(!((Train*)(BrnoToKurimStation.in))->joinTrain(this)){
				goto wait4;
			}
			t->Cancel();
			Passivate();
			if(!leaveInKurim())
				((Train*)(KurimToTisnovStation.in))->joinTrain(this);
			break;
	}
}

