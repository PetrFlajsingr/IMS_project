
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
		Id->Cancel();
		Cancel();
	} else {
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
	/*switch(Timer::getDayType(Time)){
		case WORKDAY:
			switch(Timer::getDayTime(Time)){
				case DAY:
					Activate(Time + Exponential(nextProb()));
					break;
				case NIGHT:
					Activate(Time + Exponential(nextProb()));
					break;
			}
			break;
		case WEEKEND:
			switch(Timer::getDayTime(Time)){
				case DAY:
					Activate(Time + Exponential(nextProb()));
					break;
				case NIGHT:
					Activate(Time + Exponential(nextProb()));
					break;
			}
			break;
		default: break;
	}*/
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

/**
 * Cesta cestujiciho. Odviji se podle nastupni stanice.
 */
void Commuter::Behavior(){
	//Timeout* t = new Timeout(this, Exponential(20));
	switch(this->from){
		case TISNOV: 
			TisnovToKurimQHist(Time);
			TisnovToKurimQ.Insert(this);
			wait1:
			Passivate();
			if(!((Train*)(TisnovToKurimStation.in))->joinTrain(this)){
				goto wait1;
			}
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
			Passivate();
			break;
		case KURIMB: 
			KurimToBrnoQ.Insert(this);
			wait3:
			Passivate();
			if(!((Train*)(KurimToBrnoStation.in))->joinTrain(this)){
				goto wait3;
			}
			Passivate();
			break;
		case BRNO: 
			BrnoToKurimQ.Insert(this);
			wait4:
			Passivate();
			if(!((Train*)(BrnoToKurimStation.in))->joinTrain(this)){
				goto wait4;
			}
			Passivate();
			if(!leaveInKurim())
				((Train*)(KurimToTisnovStation.in))->joinTrain(this);
			break;
	}
}

