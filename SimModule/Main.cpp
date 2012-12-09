#include "stdafx.h"
#include "Simulator.h"

int main()
{
	CSimulator Sim(0);


	Sim.SetGroupMaxMemeberNumber(3);
	
	Sim.SetMode(NO_GROUPING);


	Sim.SetVerbose(false);


	for(unsigned int RandomSeed = 0;RandomSeed < 3;RandomSeed++)
	{
		Sim.Reset(RandomSeed);

		Sim.SetEnvironmentRandomly();
		Sim.InsertWorkInsertPeer(1, 1000);
		for(int i = 0;i < 3;i++) Sim.InsertWorkSearchContent(3);
		Sim.SimulateToInfinity();
		printf("\n");

		printf("*** Simulation Statistics\n");
		printf("%s\n", Sim.GetStatistics());

		printf("*** Simulation Output\n");
		printf("%s\n", Sim.GetLog());
	}

	//while(true)
	//{
	//	Sim.SimulateCount(1);

	//	char c = _getch();
	//	if(c == 'e') break;
	//}

	return 0;
}
