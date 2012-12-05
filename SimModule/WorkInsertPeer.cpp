#include "stdafx.h"
#include "WorkInsertPeer.h"
#include "PeerInfo.h"

CWorkInsertPeer::CWorkInsertPeer(CSimulator *Simulator) : CWorkBase(Simulator)
{
}

CWorkInsertPeer::~CWorkInsertPeer()
{
}

void CWorkInsertPeer::Simulate(char *Log)
{
	char *LogPT = Log;
	if(LogPT != NULL) LogPT+= sprintf(LogPT, "Insert a Peer.");

	// Create a peer.
	CPeerInfo *PeerInfo = new CPeerInfo(++Sim->NewPeerID);

	// Set neighbor peers.
	// This method is not optimal.
	if(Sim->PeerInfoMap.GetCount() > 0)
	{
		unsigned int StartIndex = Sim->wRand() % Sim->PeerInfoMap.GetCount();
		POSITION pos = Sim->PeerInfoMap.GetStartPosition();
		for(unsigned int j = 0;j < StartIndex;j++) Sim->PeerInfoMap.GetNext(pos);
		unsigned int StartPeerID = Sim->PeerInfoMap.GetKeyAt(pos), PeerID = StartPeerID;
		for(unsigned int NeighborCount = 0;NeighborCount < Sim->InitNeighborPeerCount;)
		{
			if(PeerInfo->IsNeighborPeer(PeerID) == false)
			{
				PeerInfo->InsertNeighborPeerID(PeerID);
				NeighborCount++;
			}
			Sim->PeerInfoMap.GetNext(pos);
			if(pos == NULL) pos = Sim->PeerInfoMap.GetStartPosition();
			PeerID = Sim->PeerInfoMap.GetKeyAt(pos);
			if(PeerID == StartPeerID) break;
		}
	}

	// Insert a peer.
	Sim->PeerInfoMap.SetAt(PeerInfo->GetPeerID(), PeerInfo);

	if(LogPT != NULL)
	{
		LogPT+= sprintf(LogPT, "\n  ", PeerInfo->GetPeerID());
		LogPT+= PeerInfo->GerProperties(LogPT);
	}
}
