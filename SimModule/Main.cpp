#include "stdafx.h"
#include "Simulator.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

#define SIMULATOR_THREAD_COUNT					1
#define PEER_COUNT								10
#define SEARCH_CONTENT_COUNT					5



CwLock PrintLock;
/*
DWORD WINAPI SimulatorThread(LPVOID Argument)
{
	int ThreadID = (int)Argument;

	CSimulator Sim;
	Sim.SetVerbose(true);

	//while(true)
	{
		system("cls");

		//Sim.Reset(GetTickCount());
		Sim.Reset(0);
		
		Sim.SetEnvironmentRandomly();

		Sim.SetGroupMaxMemeberNumber(5); //jin


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

<<<<<<< HEAD
		//	system("cls");

=======
>>>>>>> 3a03ca2caaaabcdfeb9f5e9815b6abd9e684e9a6
			printf("================================ Thread %u ================================\n", ThreadID);
			printf("*** Simulation is terminated at step %u\n", Sim.Step);
			printf("*** Simulation Statistics\n");
			printf("%s", Sim.GetStatistics());

			printf("*** Simulation Output\n");
<<<<<<< HEAD
			printf("%s\n", Sim.GetLog());


		}
	}

	printf("================================ PEER GROUP INFO  ================================\n");


	POSITION pos = 	Sim.PeerInfoMap.GetStartPosition();
	while(pos != NULL)
	{
		unsigned int PeerID;
		CPeerInfo *PeerInfo;
		Sim.PeerInfoMap.GetNextAssoc(pos, PeerID, PeerInfo);
		printf("PeerID : %u , GroupID : %u , curr : %u   group_member :",PeerInfo->PeerID, PeerInfo->groupID, PeerInfo->currGroupMemberNumber	);
		POSITION pos2 = PeerInfo->GroupPeerIDMap.GetStartPosition();
		while(pos2 !=NULL)
		{
			unsigned int groupID;
			void* temp;
			PeerInfo->GroupPeerIDMap.GetNextAssoc(pos2, groupID, temp);
			printf("  %u,   ", groupID);
		}
		printf("\n   NEIGHBOR ");

		pos2 = PeerInfo->NeighborPeerIDMap.GetStartPosition();
		while(pos2 !=NULL)
		{
			unsigned int groupID;
			void* temp;
			PeerInfo->NeighborPeerIDMap.GetNextAssoc(pos2, groupID, temp);
			printf("  %u,   ", groupID);
		}
			printf("   othergroup ");
		pos2 =PeerInfo->OtherGroupPeerIDMap.GetStartPosition();
		while(pos2 !=NULL)
		{
			unsigned int groupID;
			void* temp;
			PeerInfo->OtherGroupPeerIDMap.GetNextAssoc(pos2, groupID, temp);
			printf("  %u,   ", groupID);
		}
		
		printf(" \n");
		
		
=======
			//printf("%s\n", Sim.GetLog());
		}

		Sleep(1000);
>>>>>>> 3a03ca2caaaabcdfeb9f5e9815b6abd9e684e9a6
	}

	return 0;
}
*/
int main()
{
	// Create simulator threads
	for(int i = 0;i < SIMULATOR_THREAD_COUNT;i++)
	{
//		CreateThread(NULL, 0, SimulatorThread, (LPVOID)(i + 1), 0, NULL);
	}

	while(true) Sleep(1000);

	return 0;
}
