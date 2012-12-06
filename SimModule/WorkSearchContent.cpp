#include "stdafx.h"
#include "WorkQueue.h"
#include "WorkSearchContent.h"
#include "WorkSendMessage.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

CWorkSearchContent::CWorkSearchContent(CSimulator *Simulator, unsigned int PeerID, unsigned int ContentID) : CWorkBase(Simulator)
{
	this->PeerID = PeerID;
	this->ContentID = ContentID;
}

CWorkSearchContent::~CWorkSearchContent()
{
}

void CWorkSearchContent::Simulate(char *Log)
{
	char *LogPT = Log;
	LogPT+= sprintf(LogPT, "    Peer searches a content.");

	// Get a random peer.
	CPeerInfo *PeerInfo;
	if(PeerID == SIM_RANDOM_VALUE)
	{
		PeerInfo = Sim->GetRandomPeer();
		if(PeerInfo != NULL) PeerID = PeerInfo->PeerID;
		else
		{
			LogPT+= sprintf(LogPT, "\n    Simulator cannot choice a random peer.");
			return;
		}
	}
	else
	{
		if(Sim->PeerInfoMap.Lookup(PeerID, PeerInfo) == NULL)
		{
			LogPT+= sprintf(LogPT, "\n    Simulator cannot find the peer. (PeerID = %u)", PeerID);
			return;
		}
	}

	// Get a random content.
	if(ContentID == SIM_RANDOM_VALUE)
	{
		CContentInfo *ContentInfo = Sim->GetRandomContent();
		if(ContentInfo != NULL) ContentID = ContentInfo->ContentID;
		else
		{
			LogPT+= sprintf(LogPT, "\n    Simulator cannot choice a random content.");
			return;
		}
	}

	LogPT+= sprintf(LogPT, " (PeerID = %u, ContentID = %08X, NeighborPeerIDs =", PeerID, ContentID);

	// Send the search messages to neighbors.
	CWorkQueue WorkQueue;
	POSITION pos = PeerInfo->NeighborPeerIDMap.GetStartPosition();
	if(pos != NULL) ++PeerInfo->NewMessageID;
	while(pos != NULL)
	{
		unsigned int NeighborPeerID;
		void *Temp;
		PeerInfo->NeighborPeerIDMap.GetNextAssoc(pos, NeighborPeerID, Temp);
		
		CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, PeerID, NeighborPeerID);
		WorkSendMessage->DontIncreaseWorkNumber = true;
		WorkSendMessage->Message = new CMessage(PeerInfo->NewMessageID);
		WorkSendMessage->Message->SetSearchContent(PeerID, ContentID);
		WorkQueue.QueueAtTail(WorkSendMessage);

		LogPT+= sprintf(LogPT, " %u", NeighborPeerID);
	}
	Sim->InsertWork(Sim->Step, &WorkQueue, true);
	WorkQueue.RemoveAll();

	LogPT+= sprintf(LogPT, ")\n");
}
