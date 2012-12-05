#include "stdafx.h"
#include "Simulator.h"
#include "WorkQueue.h"
#include "WorkInsertPeer.h"
#include "PeerInfo.h"

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
	InitNeighborPeerCount = 3;
}

void CSimulator::SetEnvironmentRandomly()
{
	InitNeighborPeerCount = wRand() % 5;
}

void CSimulator::SetEnvironmentManually(unsigned int InitNeighborPeerCount)
{
	this->InitNeighborPeerCount = InitNeighborPeerCount;
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
	unsigned int Number;
	while(WorkQueue->DeQueue(&Work, &Number) == true)
	{
		// Simulate a work.
		char Log[4096];
		Work->Simulate(Log);
		printf("- Work %u> %s\n", Number, Log);
		delete Work;
	}
	WorkQueueMap.RemoveKey(Step);
	delete WorkQueue;

End:
	printf("\n");

	return true;
}

void CSimulator::InsertWork(unsigned int StepNumber, CWorkBase *Work)
{
	// Is StepNumber already passed?
	if(StepNumber <= Step) return;

	// Create a WorkQueue or get an existing WorkQueue at the StepNumber.
	CWorkQueue *WorkQueue;
	if(WorkQueueMap.Lookup(StepNumber, WorkQueue) == false)
	{
		WorkQueue = new CWorkQueue();
		WorkQueueMap.SetAt(StepNumber, WorkQueue);
	}

	// Insert the Work
	WorkQueue->Queue(Work);
}
