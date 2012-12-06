#include "stdafx.h"
#include "Simulator.h"
#include "WorkQueue.h"
#include "WorkInsertPeer.h"
#include "WorkSearchContent.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

CSimulator::CSimulator(unsigned long RandomSeed)
{
	Reset(RandomSeed);
}

CSimulator::~CSimulator()
{
	// Free all allocated memory
	DeleteAllData();
}

void CSimulator::Reset(unsigned long RandomSeed)
{
	Step = 0;

	NewPeerID = 0;

	wRand.SetSeed(RandomSeed);

	// Free all allocated memory
	DeleteAllData();

	// Set environment to default 
	SetEnvironmentDefault();
}

void CSimulator::SetEnvironmentDefault()
{
	IsRandomEnrivonment = false;
	InitNeighborPeerCount = 3;
	InitContentCount = 5;
}

void CSimulator::SetEnvironmentRandomly()
{
	IsRandomEnrivonment = true;
	InitNeighborPeerCount = 1 + wRand() % 5;
	InitContentCount = wRand() % 10;
}

void CSimulator::SetEnvironmentManually(unsigned int InitNeighborPeerCount, unsigned int InitContentCount)
{
	IsRandomEnrivonment = false;
	this->InitNeighborPeerCount = InitNeighborPeerCount;
	this->InitContentCount = InitContentCount;
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
	InsertWork(StepNumber, new CWorkSearchContent(this, PeerID, ContentID));
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
		printf("*** Simulation is terminated at step %u\n", Step);
		return false;
	}

	// Increase the step that will be simulated.
	Step++;
	printf("*** Step %u\n", Step);

	// Get the WorkQueue that contains a work list at this Step.
	CWorkQueue *WorkQueue;
	if(WorkQueueMap.Lookup(Step, WorkQueue) == false)
	{
		// There is no works to simulate.
		goto End;
	}

	// Get works from the WorkQueue and simulate each work.
	CWorkBase *Work;
	unsigned int PrevNumber = 0, NextNumber;
	while(WorkQueue->DeQueue(&Work, &NextNumber) == true)
	{
		// Simulate a work.
		char Log[4096];
		Work->Simulate(Log);
		if(PrevNumber != NextNumber) printf("- Work %u>\n", NextNumber);
		printf("%s", Log);
		delete Work;
		PrevNumber = NextNumber;

		// Change the environment.
		if(IsRandomEnrivonment == true) SetEnvironmentRandomly();
	}
	WorkQueueMap.RemoveKey(Step);
	delete WorkQueue;

End:
	printf("\n");

	return true;
}
