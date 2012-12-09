#include "stdafx.h"
#include "Simulator.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

#define SIMULATOR_THREAD_COUNT					2
#define PEER_COUNT								10000

CwLock PrintLock;

DWORD WINAPI SimulatorThread(LPVOID Argument)
{
	int ThreadID = (int)Argument;

	CSimulator Sim;
	Sim.SetVerbose(false);

	//while(true)
	{
		system("cls");

		//Sim.Reset(GetTickCount());
		Sim.Reset(0);
		
		Sim.SetEnvironmentRandomly();
		Sim.SetGroupMaxMemeberNumber(3); //jin
		Sim.SetMode(ThreadID == 1 ? MODE_CACHE_OFF : MODE_CACHE_ON, MODE_GROUPING_OFF); //jin

		// Insert peers
		Sim.InsertWorkInsertPeer(1, PEER_COUNT);
		Sim.SimulateTo(2);

		// Search a content
		CContentInfo *ContentInfo = Sim.GetRandomContent();
		for(int j = 0;j < 10;j++)
		{
			Sim.InsertWorkSearchContent(Sim.Step + 1, SIM_RANDOM_VALUE, ContentInfo->ContentID);
			Sim.SimulateToInfinity();
		}

		// Result Print
		{
			CwLockAuto PrintLockAuto(&PrintLock);

			printf("================================ Thread %u ================================\n", ThreadID);
			printf("*** Simulation is terminated at step %u\n", Sim.Step);
			printf("*** Simulation Statistics\n");
			printf("%s", Sim.GetStatistics());

			printf("*** Simulation Output\n");
			//printf("%s\n", Sim.GetLog());
		}

		Sleep(1000);
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
