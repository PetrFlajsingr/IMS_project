#ifndef TRAIN_H
#define TRAIN_H

#include <simlib.h>
#include "Commuter.h"
#include "globals.h"
#include "constants.h"

using namespace simlib3;

/**
 * Generator vlaku
 */
class TrainGenerator: public Event{
	void Behavior();
};

class Train: public Process {
private:
	unsigned int capacity = TRAIN_CAPACITY; //< kapacita cestujicic 

	unsigned int delay = 0; //< celkove zpodeni pro statistiky

	bool startsInBrno; //< smer vlaku

	Queue people; //< cestujici ve vlaku

	void TisnovToBrno();

	void BrnoToTisnov();

	double generateDelay();

	void activateAllInQueue(Queue* q);

	void cancelAllInQueue(Queue* q);

	void logFullness();
public:
	Train(bool startsInBrno);

	~Train();

	void Behavior();

	bool joinTrain(Commuter* person);
};


#endif