#include "stdafx.h"
#include "Simulator.h"

#define SIMULATOR_THREAD_COUNT					1
#define PEER_COUNT								10000
#define SEARCH_CONTENT_COUNT					5

CwLock PrintLock;

DWORD WINAPI SimulatorThread(LPVOID Argument)
{

	int ThreadID = (int)Argument;
	CSimulator Sim;
	Sim.SetVerbose(false);

	while(true)
	{
		Sim.Reset(GetTickCount());
		
		Sim.SetEnvironmentRandomly();
		Sim.SetGroupMaxMemeberNumber(3); //jin
		Sim.SetMode(NO_GROUPING); //jin

		Sim.InsertWorkInsertPeer(1, PEER_COUNT);
		for(int j = 0;j < SEARCH_CONTENT_COUNT;j++) Sim.InsertWorkSearchContent(3);
		Sim.SimulateToInfinity();

		// Result Print
		{
			CwLockAuto PrintLockAuto(&PrintLock);

			system("cls");

			printf("================================ Thread %u ================================\n", ThreadID);
			printf("*** Simulation is terminated at step %u\n", Sim.Step);
			printf("*** Simulation Statistics\n");
			printf("%s", Sim.GetStatistics());

			printf("*** Simulation Output\n");
			printf("%s\n", Sim.GetLog());
		}
	}

	return 0;
}

int main()
{
	// Create simulator threads
	for(int i = 0;i < SIMULATOR_THREAD_COUNT;i++)
	{
		CreateThread(NULL, 0, SimulatorThread, (LPVOID)(i + 1), 0, NULL);
	}

	while(true) Sleep(1000);

	return 0;
}
