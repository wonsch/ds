#pragma once

#ifdef SIM_EXPORT
#define SIMULATORAPI							__declspec(dllexport)
#else
#define SIMULATORAPI							__declspec(dllimport)
#endif

#define SIM_RANDOM_VALUE						-1
#define SIM_RANDOM_NEIGHBOR_RANGE				3
#define SIM_MAX_FLOOD_HOP_COUNT					6

enum ECacheMode
{
	MODE_CACHE_OFF = 0,
	MODE_CACHE_ON,
};

enum EGroupMode
{
	MODE_GROUPING_OFF = 0,
	MODE_GROUPING_ON,
};

class SIMULATORAPI CSimulatorEx
{
public:
	////////////////////////////////////////////////////////////////
	// For user
	////////////////////////////////////////////////////////////////
	CSimulatorEx(unsigned long RandomSeed = 0);
	~CSimulatorEx();

	void										SetVerbose(bool Verbose);
	void										SetMode(ECacheMode CacheMode, EGroupMode GroupMode);

	void										Reset(unsigned long RandomSeed = 0);
	void										SetEnvironmentDefault();
	void										SetEnvironmentRandomly();
	void										SetEnvironmentManually(unsigned int InitNeighborPeerCount, unsigned int InitContentCount, unsigned int InitMaxFloodHopCount);

	void										SimulateCount(unsigned int StepCount = 1);
	void										SimulateTo(unsigned int StepNumber);
	void										SimulateToInfinity();

	void										InsertWorkInsertPeer(unsigned int StepNumber, unsigned int PeerCount);
	void										InsertWorkSearchContent(unsigned int StepNumber, unsigned int PeerID = SIM_RANDOM_VALUE, unsigned int ContentID = SIM_RANDOM_VALUE);

	void										DumpOpen();
	void										DumpFinal();
	void										DumpPeers();

	////////////////////////////////////////////////////////////////
	// For simulator
	////////////////////////////////////////////////////////////////
	unsigned int								GetStep();
	class CPeerInfo*							GetRandomPeer();
	class CContentInfo*							GetRandomContent();

	/*   JIN    */
	void										SetGroupMaxMemeberNumber(unsigned int MaxNumber);					
	void										SetContentInfoFloodingTTL(int TTL);
	/*   JIN    */

	unsigned int								GetStatisticsTotalSearchContentCount();
	unsigned int								GetStatisticsTotalSearchContentSuccessCount();
	unsigned int								GetStatisticsTotalSearchContentHopCount();
	unsigned int								GetStatisticsTotalMessageCount();
	CAtlString									GetStatistics();
	CAtlString&									GetLog();

private:
	class CSimulatorIn*							SI;
};
