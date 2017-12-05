#ifndef COMMUTER_H
#define COMMUTER_H

#include <simlib.h>
#include "globals.h"
#include "Timer.h"
using namespace simlib3;
class CommuterGenerator : public Event{
private:
	STATION from; //< pocatecni stanice cestujiciho

	double nextProb();
public:
	CommuterGenerator(STATION from);
	void Behavior();
};

class Commuter : public Process{
private:
	STATION from; //< pocatecni stanice cestujiciho

	bool leaveInKurim();
public:
	void Behavior();

	Commuter(STATION from);
};

class Timeout : public Event{
private:
	Commuter* Id;
	double dt;
public:
	Timeout(Commuter* person, double dt);

	void Behavior();
};
#endif