#include <simlib.h>
#include "globals.h"
#include "constants.h"
#include "Train.h"
#include "Commuter.h"
#include <string>

using namespace simlib3;


int main(){
	std::string outFile = "out.out";
	SetOutput(outFile.c_str());

	Init(0, SIMULATION_END_TIME);
	
	(new TrainGenerator())->Activate();
	(new CommuterGenerator(TISNOV))->Activate();
	(new CommuterGenerator(KURIMT))->Activate();
	(new CommuterGenerator(KURIMB))->Activate();
	(new CommuterGenerator(BRNO))->Activate();


	Run();

	trainStat.Output();
	TisnovToKurimQHist.Output();
	trainFullnessStatWorkDay.Output();
 	trainFullnessStatWorkNight.Output();
 	trainFullnessStatWeekendDay.Output();
 	trainFullnessStatWeekendNight.Output();

	return 0;
}