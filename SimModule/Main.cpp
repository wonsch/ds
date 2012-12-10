#include "stdafx.h"
#include "Simulator.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

#define SIMULATOR_THREAD_COUNT					3
//#define DUMP
#define PEER_COUNT								10000
#define GROUP_SIZE								10
#define GROUP_COUNT								((PEER_COUNT + (GROUP_SIZE - 1)) / GROUP_SIZE)
#define SEARCH_CONTENT_COUNT					10
#define CONTENT_INFO_FLOOD_TTL					GROUP_COUNT

CwLock PrintLock;
int PrintTurn = 1;
unsigned int GlobalRandomSeed;
DWORD LastThreadPrintTime = 0;
bool IsFirstSimulation = true;

DWORD WINAPI SimulatorThread(LPVOID Argument)
{
	int ThreadID = (int)Argument;

	CSimulator Sim;
	Sim.SetVerbose(false);

	for(unsigned int RandomSeed = GlobalRandomSeed;;RandomSeed++)
	//while(true)
	{
		//unsigned int RandomSeed = GetTickCount();
		//for(int i = 0;i < 3;i++)
		{
			Sim.Reset(RandomSeed);
			//Sim.Reset(0);
			
			Sim.SetEnvironmentRandomly();

			switch(ThreadID)
			{
			case 1:
				Sim.SetMode(MODE_CACHE_OFF, MODE_GROUPING_OFF);
				break;
			case 2:
				Sim.SetMode(MODE_CACHE_ON, MODE_GROUPING_OFF);
				break;
			case 3:
				Sim.SetMode(MODE_CACHE_ON, MODE_GROUPING_ON);
				Sim.SetGroupMaxMemeberNumber(GROUP_SIZE); //jin
				Sim.SetContentInfoFloodingTTL(CONTENT_INFO_FLOOD_TTL); //jin
				break;
			}
#ifdef DUMP
			Sim.DumpOpen();
#endif

			DWORD StartTime = GetTickCount();

			// Insert peers
			for(int j = 0;j < PEER_COUNT / 5;j++) Sim.InsertWorkInsertPeer(j + 1, 5);
			Sim.SimulateToInfinity();

			// Search a content
			CContentInfo *ContentInfo = Sim.GetRandomContent();
			for(int j = 0;j < SEARCH_CONTENT_COUNT;j++)
			{
				//if(ThreadID == 3) Sim.SetVerbose(true);

				Sim.InsertWorkSearchContent(Sim.Step + 1, SIM_RANDOM_VALUE, ContentInfo->ContentID);
				Sim.SimulateToInfinity();
			}

			DWORD EndTime = GetTickCount();

			// Result Print
			while(PrintTurn != ThreadID) Sleep(1);

			{
				CwLockAuto PrintLockAuto(&PrintLock);

				if(ThreadID == 1)
				{
					if(IsFirstSimulation == false)
					{
						printf("\n*** Press any key to see the next simulation. ***\n");
						Sleep(3000);
						//_getch();
					}
					else IsFirstSimulation = false;

					/*DWORD Timer = GetTickCount();
					if(LastThreadPrintTime + 1000 >= Timer)
					{
						Sleep(LastThreadPrintTime + 1000 - Timer);
					}*/
					system("cls");
				}

				switch(ThreadID)
				{
				case 1:
					printf("================================ Pure P2P ================================\n");
					break;
				case 2:
					printf("================================ Key Paper ================================\n");
					break;
				case 3:
					printf("================================ Our Work ================================\n");
					break;
				}
//				printf("================================ Thread %u ================================\n", ThreadID);
				printf("*** Simulation is terminated at step %u (%.1fs)\n", Sim.Step, (double)(EndTime - StartTime) / 1000);
				printf("*** Simulation Statistics\n");
				printf("%s", Sim.GetStatistics());

				printf("*** Simulation Output\n");
				//printf("%s\n", Sim.GetLog());

				// jin
				/*printf("================================ PEER GROUP INFO  ================================\n");

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
				}*/
			}
			if(PrintTurn == SIMULATOR_THREAD_COUNT) LastThreadPrintTime = GetTickCount();
			PrintTurn = PrintTurn % SIMULATOR_THREAD_COUNT + 1;

			// Final Dump
#ifdef DUMP
			Sim.DumpFinal();
#endif
		}

		//_getch();
		//Sleep(5000);
	}

	return 0;
}

int main()
{
	GlobalRandomSeed = GetTickCount();

	// Create simulator threads
	for(int i = 0;i < SIMULATOR_THREAD_COUNT;i++)
	{
		CreateThread(NULL, 0, SimulatorThread, (LPVOID)(i + 1), 0, NULL);
	}

	while(true) Sleep(1000);

	return 0;
}
