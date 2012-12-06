#include "stdafx.h"
#include "WorkSendMessage.h"
#include "WorkRecvMessage.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

CWorkRecvMessage::CWorkRecvMessage(CSimulator *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID) : CWorkMessageBase(Simulator, SrcPeerID, DstPeerID)
{
}

CWorkRecvMessage::~CWorkRecvMessage()
{
}

void CWorkRecvMessage::Simulate(char *Log)
{
	assert(Message != NULL);

	char *LogPT = Log;
	LogPT+= sprintf(LogPT, "    Receive a message. (SrcPeerID = %u, DstPeerID = %u, MessageID = %u", SrcPeerID, DstPeerID, Message->MessageID);

	switch(Message->Message)
	{
	case EMESSAGE_EMPTY:
		// Empty
		break;

	case EMESSAGE_NOTIFY_NULL:
		// Notify NULL
		{
			LogPT+= sprintf(LogPT, ", Message = NotifyNull");

			// Get the destination peer.
			CPeerInfo *DstPeerInfo;
			if(Sim->PeerInfoMap.Lookup(DstPeerID, DstPeerInfo) == false) break;
			DstPeerInfo->InsertNeighborPeerID(SrcPeerID);

			LogPT+= sprintf(LogPT, ")\n    PeerID %u inserted the PeerID %u into its neighbor list. (NeighborPeerIDs = ", DstPeerID, SrcPeerID);
			LogPT+= DstPeerInfo->GetNeighborPeerIDs(LogPT);
		}
		break;

	case EMESSAGE_SEARCH_CONTENT:
		// Search a content
		{
			LogPT+= sprintf(LogPT, ", Message = SearchContent, FromPeerID = %u, ContentID = %08X", Message->FromPeerID, Message->ContentID);

			// Get the destination peer.
			CPeerInfo *DstPeerInfo;
			if(Sim->PeerInfoMap.Lookup(DstPeerID, DstPeerInfo) == false) break;

			// TODO:
			// Should the peer transit this message to its neighbors?
			// ...

			// Send the search messages to neighbors.
			LogPT+= sprintf(LogPT, ")\n    Transit a message. (NeighborPeerIDs =");
			POSITION pos = DstPeerInfo->NeighborPeerIDMap.GetStartPosition();
			while(pos != NULL)
			{
				unsigned int NeighborPeerID;
				void *Temp;
				DstPeerInfo->NeighborPeerIDMap.GetNextAssoc(pos, NeighborPeerID, Temp);

				if(NeighborPeerID != SrcPeerID && NeighborPeerID != Message->FromPeerID)
				{
					CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, NeighborPeerID);
					WorkSendMessage->Message = new CMessage(Message->MessageID);
					WorkSendMessage->Message->SetSearchContent(Message->FromPeerID, Message->ContentID);
					Sim->InsertWork(Sim->Step + 1, WorkSendMessage);

					LogPT+= sprintf(LogPT, " %u", NeighborPeerID);
				}
			}
		}
		break;
	}

	LogPT+= sprintf(LogPT, ")\n");
}
