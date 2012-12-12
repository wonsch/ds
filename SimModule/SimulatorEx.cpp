#include "stdafx.h"
#include "SimulatorIn.h"
#include "SimulatorEx.h"

CSimulatorEx::CSimulatorEx(unsigned long RandomSeed)
{
	SI = new CSimulatorIn(RandomSeed);
}

CSimulatorEx::~CSimulatorEx()
{
	if(SI != NULL) delete SI;
}

void CSimulatorEx::SetVerbose(bool Verbose)
{
	SI->SetVerbose(Verbose);
}

void CSimulatorEx::SetMode(ECacheMode CacheMode, EGroupMode GroupMode)
{
	SI->SetMode(CacheMode, GroupMode);
}

void CSimulatorEx::Reset(unsigned long RandomSeed)
{
	SI->Reset(RandomSeed);
}

void CSimulatorEx::SetEnvironmentDefault()
{
	SI->SetEnvironmentDefault();
}

void CSimulatorEx::SetEnvironmentRandomly()
{
	SI->SetEnvironmentRandomly();
}

void CSimulatorEx::SetEnvironmentManually(unsigned int InitNeighborPeerCount, unsigned int InitContentCount, unsigned int InitMaxFloodHopCount)
{
	SI->SetEnvironmentManually(InitNeighborPeerCount, InitContentCount, InitMaxFloodHopCount);
}

void CSimulatorEx::SimulateCount(unsigned int StepCount)
{
	SI->SimulateCount(StepCount);
}

void CSimulatorEx::SimulateTo(unsigned int StepNumber)
{
	SI->SimulateTo(StepNumber);
}

void CSimulatorEx::SimulateToInfinity()
{
	SI->SimulateToInfinity();
}

void CSimulatorEx::InsertWorkInsertPeer(unsigned int StepNumber, unsigned int PeerCount)
{
	SI->InsertWorkInsertPeer(StepNumber, PeerCount);
}

void CSimulatorEx::InsertWorkSearchContent(unsigned int StepNumber, unsigned int PeerID, unsigned int ContentID)
{
	SI->InsertWorkSearchContent(StepNumber, PeerID, ContentID);
}

void CSimulatorEx::DumpOpen()
{
	SI->DumpOpen();
}

void CSimulatorEx::DumpFinal()
{
	SI->DumpFinal();
}

void CSimulatorEx::DumpPeers()
{
	SI->DumpPeers();
}

unsigned int CSimulatorEx::GetStep()
{
	return SI->Step;
}

CPeerInfo *CSimulatorEx::GetRandomPeer()
{
	return SI->GetRandomPeer();
}

CContentInfo *CSimulatorEx::GetRandomContent()
{
	return SI->GetRandomContent();
}

void CSimulatorEx::SetMaxFloodHopCount(unsigned int MaxFloodHopCount)
{
	SI->SetMaxFloodHopCount(MaxFloodHopCount);
}

void CSimulatorEx::SetGroupMaxMemeberNumber(unsigned int MaxNumber)
{
	SI->SetGroupMaxMemeberNumber(MaxNumber);
}

void CSimulatorEx::SetContentInfoFloodingTTL(int TTL)
{
	SI->SetContentInfoFloodingTTL(TTL);
}

unsigned int CSimulatorEx::GetMessageKind()
{
	return SI->GetMessageKind();
}

unsigned int CSimulatorEx::GetStatisticsTotalSearchContentCount()
{
	return SI->GetStatisticsTotalSearchContentCount();
}

unsigned int CSimulatorEx::GetStatisticsTotalSearchContentSuccessCount()
{
	return SI->GetStatisticsTotalSearchContentSuccessCount();
}

unsigned int CSimulatorEx::GetStatisticsTotalSearchContentHopCount()
{
	return SI->GetStatisticsTotalSearchContentHopCount();
}

unsigned int CSimulatorEx::GetStatisticsTotalMessageCount()
{
	return SI->GetStatisticsTotalMessageCount();
}

unsigned int CSimulatorEx::GetStatisticsTotalMessageCountEach(int Index)
{
	return SI->GetStatisticsTotalMessageCountEach(Index);
}

CAtlString CSimulatorEx::GetStatistics()
{
	return SI->GetStatistics();
}

CAtlString &CSimulatorEx::GetLog()
{
	return SI->GetLog();
}
