#pragma once

class CWorkQueue;
class CWorkBase;
class CPeerInfo;

class CSimulator
{
public:
	CSimulator(unsigned long RandomSeed = 0);
	~CSimulator();
	
	void										Reset(unsigned long RandomSeed = 0);
	void										SetEnvironmentDefault();
	void										SetEnvironmentRandomly();
	void										SetEnvironmentManually(unsigned int InitNeighborPeerCount);

	void										SimulateCount(unsigned int StepCount = 1);
	void										SimulateTo(unsigned int StepNumber);
	void										SimulateToInfinity() {SimulateTo(-1);}

	void										InsertWorkInsertPeer(unsigned int StepNumber, unsigned int PeerCount);

private:
	void										DeleteAllData();

	bool										SimulateOneStep();

	void										InsertWork(unsigned int StepNumber, CWorkBase *Work);

public:
	////////////////////////////////////////////////////////////////
	// Environment
	////////////////////////////////////////////////////////////////
	unsigned int								InitNeighborPeerCount;

	////////////////////////////////////////////////////////////////
	// Simulation data
	////////////////////////////////////////////////////////////////
	unsigned int								Step;
	CAtlMap<unsigned int, CWorkQueue*>			WorkQueueMap; // Map<Step, WorkQueue>

	unsigned int								NewPeerID;
	CAtlMap<unsigned int, CPeerInfo*>			PeerInfoMap; // Map<PeerID, PeerInfo>

	CwRand										wRand;
};
