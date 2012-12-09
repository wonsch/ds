#pragma once

#define SIM_RANDOM_VALUE						-1
#define SIM_RANDOM_NEIGHBOR_RANGE				3

class CWorkQueue;
class CWorkBase;
class CPeerInfo;
class CContentInfo;


enum EMode
{
	NO_GROUPING = 0,
	GROUPING,
};

class CSimulator
{
public:
	////////////////////////////////////////////////////////////////
	// For user
	////////////////////////////////////////////////////////////////
	CSimulator(unsigned long RandomSeed = 0);
	~CSimulator();

	void										SetVerbose(bool Verbose) {this->Verbose = Verbose;}
	
	void										Reset(unsigned long RandomSeed = 0);
	void										SetEnvironmentDefault();
	void										SetEnvironmentRandomly();
	void										SetEnvironmentManually(unsigned int InitNeighborPeerCount, unsigned int InitContentCount, unsigned int InitMaxFloodHopCount);

	void										SimulateCount(unsigned int StepCount = 1);
	void										SimulateTo(unsigned int StepNumber);
	void										SimulateToInfinity() {SimulateTo(-1);}

	void										InsertWorkInsertPeer(unsigned int StepNumber, unsigned int PeerCount);
	void										InsertWorkSearchContent(unsigned int StepNumber, unsigned int PeerID = SIM_RANDOM_VALUE, unsigned int ContentID = SIM_RANDOM_VALUE);

	////////////////////////////////////////////////////////////////
	// For simulator
	////////////////////////////////////////////////////////////////
	void										InsertWork(unsigned int StepNumber, CWorkQueue *Work, bool AtHead = false);
	void										InsertWork(unsigned int StepNumber, CWorkBase *Work, bool AtHead = false);

	CPeerInfo*									GetRandomPeer();
	CContentInfo*								GetRandomContent();

	/*   JIN    */

	void										SetGroupMaxMemeberNumber(unsigned int MaxNumber);					

	unsigned int								GroupMaxNumber;
	void										SetMode(int m);
	int											mode;

	/*   JIN    */

	CAtlString									GetStatistics();

	void										AttachLog(CAtlString &String);
	CAtlString&									GetLog();


private:
	void										DeleteAllData();

	bool										SimulateOneStep();

	bool										Verbose;

	CAtlString									Log;

public:
	////////////////////////////////////////////////////////////////
	// Environment
	////////////////////////////////////////////////////////////////
	bool										IsRandomEnrivonment;
	unsigned int								InitNeighborPeerCount;
	unsigned int								InitContentCount;
	unsigned int								InitMaxFloodHopCount;


	////////////////////////////////////////////////////////////////
	// Simulation data
	////////////////////////////////////////////////////////////////
	unsigned int								Step;
	CAtlMap<unsigned int, CWorkQueue*>			WorkQueueMap; // Map<Step, WorkQueue>

	unsigned int								NewPeerID;
	CAtlMap<unsigned int, CPeerInfo*>			PeerInfoMap; // Map<PeerID, PeerInfo>

	CAtlMap<unsigned int, CContentInfo*>		ContentInfoMap; // Map<ContentID, ContentInfo>

	CwRand										wRand;


	unsigned int								StatisticsTotalSearchContentCount;
	unsigned int								StatisticsTotalSearchContentSuccessCount;
	unsigned int								StatisticsTotalSearchContentHopCount;
	unsigned int								StatisticsTotalMessageCount;

};
