#include "stdafx.h"
#include "WorkQueue.h"
#include "WorkInsertPeer.h"
#include "WorkSendMessage.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

CWorkInsertPeer::CWorkInsertPeer(CSimulator *Simulator) : CWorkBase(Simulator)
{
}

CWorkInsertPeer::~CWorkInsertPeer()
{
}

void CWorkInsertPeer::Simulate(char *Log)
{
	char *LogPT = Log;
	LogPT+= sprintf(LogPT, "    Insert a Peer. ");

	// Create a peer.
	CPeerInfo *PeerInfo = new CPeerInfo(++Sim->NewPeerID);

	// Set neighbor peers.
	// This is not optimal.
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

	// Set contents.
	for(unsigned int ContentCount = 0;ContentCount < Sim->InitContentCount;ContentCount++)
	{
		// Create or get a random content.
		unsigned int ContentID = Sim->wRand();
		CContentInfo *ContentInfo;
		if(Sim->ContentInfoMap.Lookup(ContentID, ContentInfo) == false)
		{
			ContentInfo = new CContentInfo(ContentID);
			Sim->ContentInfoMap.SetAt(ContentID, ContentInfo);
		}

		// Insert a content.
		PeerInfo->InsertContentInfo(ContentID, ContentInfo);
	}

	// Insert a peer.
	Sim->PeerInfoMap.SetAt(PeerInfo->PeerID, PeerInfo);

	// Notify neighbor peers.
	CWorkQueue WorkQueue;
	POSITION pos = PeerInfo->NeighborPeerIDMap.GetStartPosition();
	while(pos != NULL)
	{
		unsigned int NeighborPeerID;
		void *Temp;
		PeerInfo->NeighborPeerIDMap.GetNextAssoc(pos, NeighborPeerID, Temp);

		CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, PeerInfo->PeerID, NeighborPeerID);
		WorkSendMessage->DontIncreaseWorkNumber = true;
		WorkSendMessage->Message = new CMessage(++PeerInfo->NewMessageID);
		WorkSendMessage->Message->SetNotifyNull();
		WorkQueue.QueueAtTail(WorkSendMessage);
	}
	Sim->InsertWork(Sim->Step, &WorkQueue, true);
	WorkQueue.RemoveAll();

	LogPT+= PeerInfo->GerProperties(LogPT);
	LogPT+= sprintf(LogPT, "\n");
}
