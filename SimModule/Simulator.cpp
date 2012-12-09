#include "stdafx.h"
#include "Simulator.h"
#include "WorkQueue.h"
#include "WorkInsertPeer.h"
#include "WorkSearchContent.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

CSimulator::CSimulator(unsigned long RandomSeed)
{
	Verbose = true;
	DumpFile = NULL;

	Reset(RandomSeed);
}

CSimulator::~CSimulator()
{
	if(DumpFile != NULL) fclose(DumpFile);

	// Free all allocated memory
	DeleteAllData();
}

/*=====================JIN=======================*/
void CSimulator::SetGroupMaxMemeberNumber(unsigned int MaxNumber)
{
	GroupMaxNumber = MaxNumber;
}
/*=====================JIN=======================*/

void CSimulator::SetMode(ECacheMode CacheMode, EGroupMode GroupMode)
{
	this->CacheMode = CacheMode;
	this->GroupMode = GroupMode;
}

void CSimulator::Reset(unsigned long RandomSeed)
{
	CacheMode = MODE_CACHE_OFF;
	GroupMode = MODE_GROUPING_OFF;

	Log = "";

	Step = 0;

	NewPeerID = 0;
	
	InitRandomSeed = RandomSeed;
	wRand.SetSeed(RandomSeed);

	StatisticsTotalSearchContentCount = 0;
	StatisticsTotalSearchContentSuccessCount = 0;
	StatisticsTotalSearchContentHopCount = 0;
	StatisticsTotalMessageCount = 0;

	// Free all allocated memory
	DeleteAllData();

	// Set environment to default 
	SetEnvironmentDefault();

	/*JIN*/
	GroupMaxNumber = 0;
}

void CSimulator::SetEnvironmentDefault()
{
	IsRandomEnrivonment = false;
	InitNeighborPeerCount = 1;
	InitContentCount = 5;
	InitMaxFloodHopCount = SIM_MAX_FLOOD_HOP_COUNT;
}

void CSimulator::SetEnvironmentRandomly()
{
	IsRandomEnrivonment = true;
	InitNeighborPeerCount = SIM_RANDOM_VALUE;
	InitContentCount = SIM_RANDOM_VALUE;
	InitMaxFloodHopCount = SIM_MAX_FLOOD_HOP_COUNT;
}

void CSimulator::SetEnvironmentManually(unsigned int InitNeighborPeerCount, unsigned int InitContentCount, unsigned int InitMaxFloodHopCount)
{
	IsRandomEnrivonment = false;
	this->InitNeighborPeerCount = InitNeighborPeerCount;
	this->InitContentCount = InitContentCount;
	this->InitMaxFloodHopCount = InitMaxFloodHopCount;
}

void CSimulator::SimulateCount(unsigned int StepCount)
{
	for(unsigned int i = 0;i < StepCount;i++)
	{
		if(SimulateOneStep() == false) break;
	}
}

void CSimulator::SimulateTo(unsigned int StepNumber)
{
	for(unsigned int i = Step;i < StepNumber;i++)
	{
		if(SimulateOneStep() == false) break;
	}
}

void CSimulator::InsertWorkInsertPeer(unsigned int StepNumber, unsigned int PeerCount)
{
	for(unsigned int i = 0;i < PeerCount;i++) InsertWork(StepNumber, new CWorkInsertPeer(this));
}

void CSimulator::InsertWorkSearchContent(unsigned int StepNumber, unsigned int PeerID, unsigned int ContentID)
{
	StatisticsTotalSearchContentCount++;

	InsertWork(StepNumber, new CWorkSearchContent(this, PeerID, ContentID));
}

void CSimulator::DumpOpen()
{
	// Open a dump file
	if(DumpFile != NULL) fclose(DumpFile);
	char FileName[MAX_PATH];
	sprintf(FileName, "Sim_%08X_CACHE_%s_GROUP_%s.log", InitRandomSeed, CacheMode == MODE_CACHE_OFF ? "OFF" : "ON", GroupMode == MODE_GROUPING_OFF ? "OFF" : "ON");
	DumpFile = fopen(FileName, "w");
	assert(DumpFile != NULL);

	fprintf(DumpFile, "; Dump for simulator\n");
}

void CSimulator::DumpFinal()
{
	if(DumpFile == NULL) return;

	fprintf(DumpFile, "\n[SIMULATIONINFO]\n");
	fprintf(DumpFile, "PEERCOUNT=%u\n", PeerInfoMap.GetCount());
	fprintf(DumpFile, "CYCLECOUNT=%u\n", Step);
}

void CSimulator::DumpPeers()
{
	if(DumpFile == NULL) return;

	unsigned int PeerCount = 0;
	POSITION pos1 = PeerInfoMap.GetStartPosition();
	while(pos1 != NULL)
	{
		unsigned int PeerID;
		CPeerInfo *PeerInfo;
		PeerInfoMap.GetNextAssoc(pos1, PeerID, PeerInfo);

		fprintf(DumpFile, "\n[CYCLE%u.PEER%u]\n", Step, ++PeerCount);
		fprintf(DumpFile, "PID=%u\n", PeerID);
		fprintf(DumpFile, "NEIGHBOR=");

		bool IsFirst = true;
		POSITION pos2 = PeerInfo->NeighborPeerIDMap.GetStartPosition();
		while(pos2 != NULL)
		{
			unsigned int PeerID;
			void *Temp;
			PeerInfo->NeighborPeerIDMap.GetNextAssoc(pos2, PeerID, Temp);

			fprintf(DumpFile, "%s%u", IsFirst ? "" : "|", PeerID);
			if(IsFirst == true) IsFirst = false;
		}
		if(IsFirst == false) fprintf(DumpFile, "\n");
	}
}

void CSimulator::InsertWork(unsigned int StepNumber, CWorkQueue *Work, bool AtHead)
{
	// Is StepNumber already passed?
	if(StepNumber < Step) return;

	// Create a WorkQueue or get an existing WorkQueue at the StepNumber.
	CWorkQueue *WorkQueue;
	if(WorkQueueMap.Lookup(StepNumber, WorkQueue) == false)
	{
		WorkQueue = new CWorkQueue();
		WorkQueueMap.SetAt(StepNumber, WorkQueue);
	}

	// Insert the Work
	if(AtHead == true) WorkQueue->QueueAtHead(Work);
	else WorkQueue->QueueAtTail(Work);
}

void CSimulator::InsertWork(unsigned int StepNumber, CWorkBase *Work, bool AtHead)
{
	// Is StepNumber already passed?
	if(StepNumber < Step) return;

	// Create a WorkQueue or get an existing WorkQueue at the StepNumber.
	CWorkQueue *WorkQueue;
	if(WorkQueueMap.Lookup(StepNumber, WorkQueue) == false)
	{
		WorkQueue = new CWorkQueue();
		WorkQueueMap.SetAt(StepNumber, WorkQueue);
	}

	// Insert the Work
	if(AtHead == true) WorkQueue->QueueAtHead(Work);
	else WorkQueue->QueueAtTail(Work);
}

CPeerInfo *CSimulator::GetRandomPeer()
{
	// Get a random peer which exists.
	POSITION pos = PeerInfoMap.GetStartPosition();
	if(pos == NULL) return NULL;

	unsigned int StartIndex = wRand() % PeerInfoMap.GetCount();
	for(unsigned int i = 0;i < StartIndex;i++) PeerInfoMap.GetNext(pos);

	unsigned int PeerID;
	CPeerInfo *PeerInfo;
	PeerInfoMap.GetNextAssoc(pos, PeerID, PeerInfo);

	return PeerInfo;
}

CContentInfo *CSimulator::GetRandomContent()
{
	// Get a random content which exists.
	POSITION pos = ContentInfoMap.GetStartPosition();
	if(pos == NULL) return NULL;

	unsigned int StartIndex = wRand() % ContentInfoMap.GetCount();
	for(unsigned int i = 0;i < StartIndex;i++) ContentInfoMap.GetNext(pos);

	unsigned int ContentID;
	CContentInfo *ContentInfo;
	ContentInfoMap.GetNextAssoc(pos, ContentID, ContentInfo);

	return ContentInfo;
}

CAtlString CSimulator::GetStatistics()
{
	unsigned int TotalNeighborCount = 0;
	POSITION pos = PeerInfoMap.GetStartPosition();
	while(pos != NULL)
	{
		TotalNeighborCount+= PeerInfoMap.GetValueAt(pos)->NeighborPeerIDMap.GetCount();
		PeerInfoMap.GetNext(pos);
	}

	CAtlString String;
	String.AppendFormat("RandomSeed : %u\n", InitRandomSeed);
	String.AppendFormat("Total Peers : %u\n", PeerInfoMap.GetCount());
	//String.AppendFormat("Total Contents : %u\n", ContentInfoMap.GetCount());
	String.AppendFormat("Average Number of Contents per Peer : %g\n", (double)ContentInfoMap.GetCount() / PeerInfoMap.GetCount());
	//String.AppendFormat("Total Neighbor Peers : %u\n", TotalNeighborCount);
	String.AppendFormat("Average Number of Neighbor per Peer : %g\n", (double)TotalNeighborCount / PeerInfoMap.GetCount());
	String.AppendFormat("Total Message Count (Traffic) : %u\n", StatisticsTotalMessageCount);
	String.AppendFormat("Total Search Content : %u\n", StatisticsTotalSearchContentCount);
	String.AppendFormat("Total Search Content Success/Failure : %u / %u\n", StatisticsTotalSearchContentSuccessCount, StatisticsTotalSearchContentCount - StatisticsTotalSearchContentSuccessCount);
	if(StatisticsTotalSearchContentSuccessCount > 0)
		String.AppendFormat("Total Search Content Average Hops : %g\n", (double)StatisticsTotalSearchContentHopCount / StatisticsTotalSearchContentSuccessCount);

	return String;
}

void CSimulator::AttachLog(CAtlString &String)
{
	Log+= String;
}

CAtlString &CSimulator::GetLog()
{
	return Log;
}

void CSimulator::DeleteAllData()
{
	// Delete all WorkQueues.
	POSITION pos = WorkQueueMap.GetStartPosition();
	while(pos != NULL)
	{
		unsigned int Step;
		CWorkQueue *WorkQueue;
		WorkQueueMap.GetNextAssoc(pos, Step, WorkQueue);
		delete WorkQueue;
	}
	WorkQueueMap.RemoveAll();

	// Delete all PeerInfos.
	pos = PeerInfoMap.GetStartPosition();
	while(pos != NULL)
	{
		unsigned int PeerID;
		CPeerInfo *PeerInfo;
		PeerInfoMap.GetNextAssoc(pos, PeerID, PeerInfo);
		delete PeerInfo;
	}
	PeerInfoMap.RemoveAll();

	// Delete all PeerInfos.
	pos = ContentInfoMap.GetStartPosition();
	while(pos != NULL)
	{
		unsigned int ContentID;
		CContentInfo *ContentInfo;
		ContentInfoMap.GetNextAssoc(pos, ContentID, ContentInfo);
		delete ContentInfo;
	}
	ContentInfoMap.RemoveAll();
}

bool CSimulator::SimulateOneStep()
{
	if(WorkQueueMap.GetCount() == 0)
	{
		// There is no work to simulate.
		if(Verbose == true) printf("*** Simulation is terminated at step %u\n", Step);
		return false;
	}

	// Increase the step that will be simulated.
	Step++;
	if(Verbose == true) printf("*** Step %u\n", Step);

	// Get the WorkQueue that contains a work list at this Step.
	CWorkQueue *WorkQueue;
	if(WorkQueueMap.Lookup(Step, WorkQueue) == false)
	{
		// There is no works to simulate.
		goto End;
	}

	// Get works from the WorkQueue and simulate each work.
	CWorkBase *Work;
	unsigned int PrevNumber = 0, NextNumber, DumpNumber = 0;
	while(WorkQueue->DeQueue(&Work, &NextNumber) == true)
	{
		// Simulate a work.
		char Log[32768], Dump[32768];
		Dump[0] = 0;
		Work->Simulate(Log, Dump);
		if(Verbose == true)
		{
			if(PrevNumber != NextNumber) printf("- Work %u>\n", NextNumber);
			printf("%s", Log);
		}
		delete Work;
		PrevNumber = NextNumber;

		// Write a dump for this step
		if(DumpFile != NULL && Dump[0] != NULL)
		{
			fprintf(DumpFile, "\n[CYCLE%u.EVENT%u]\n%s", Step, ++DumpNumber, Dump);
		}

		// Change the environment.
		if(IsRandomEnrivonment == true) SetEnvironmentRandomly();
	}
	WorkQueueMap.RemoveKey(Step);
	delete WorkQueue;

	// Dump this step info.
	if(DumpFile != NULL)
	{
		fprintf(DumpFile, "\n[CYCLE%u]\n", Step);
		fprintf(DumpFile, "PEERNUM=%u\n", PeerInfoMap.GetCount());
		fprintf(DumpFile, "EVENTNUM=%u\n", DumpNumber);

		// Dump peers info.
		DumpPeers();
	}

End:
	if(Verbose == true) printf("\n");

	return true;
}
