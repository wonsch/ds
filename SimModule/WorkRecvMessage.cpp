#include "stdafx.h"
#include "WorkQueue.h"
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
			LogPT+= sprintf(LogPT, ", Message = SearchContent, FromPeerID = %u, ContentID = %08X, FloodPath =", Message->FromPeerID, Message->ContentID);
			for(POSITION pos = Message->FloodPath.GetHeadPosition();pos != NULL;) LogPT+= sprintf(LogPT, " %u", Message->FloodPath.GetNext(pos));

			// Get the destination peer.
			CPeerInfo *DstPeerInfo;
			if(Sim->PeerInfoMap.Lookup(DstPeerID, DstPeerInfo) == false) break;
			DstPeerInfo->InsertNeighborPeerID(SrcPeerID);

			// Should the peer transit this message to its neighbors?
			// = Has the peer processed this message before?
			if(DstPeerInfo->TryInsertPeerIDMessageID(Message->FromPeerID, Message->MessageID) == false) break;

			// Does the peer have the content?
			// TODO: 진영 - 현재 내가 갖고 있나만 확인하는데, 캐시에서도 있나 확인하도록 추가
			CContentInfo *ContentInfo;
			if(DstPeerInfo->ContentInfoMap.Lookup(Message->ContentID, ContentInfo) == true)
			{
				CWorkQueue WorkQueue;

				// Response to a peer which made a original request.
				CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, Message->FromPeerID);
				WorkSendMessage->Message = new CMessage(DstPeerInfo->GetNewMessageID());
				WorkSendMessage->Message->SetSearchContentResponseSource(DstPeerInfo->PeerID, Message->ContentID);
				WorkQueue.QueueAtTail(WorkSendMessage);

				// Response to a previous peer to transmit.
				WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, SrcPeerID);
				WorkSendMessage->Message = new CMessage(DstPeerInfo->GetNewMessageID());
				WorkSendMessage->Message->SetSearchContentResponsePath(DstPeerInfo->PeerID, Message->ContentID, &Message->FloodPath, DstPeerInfo->PeerID);
				WorkQueue.QueueAtTail(WorkSendMessage);

				Sim->InsertWork(Sim->Step, &WorkQueue, true);
				WorkQueue.RemoveAll();

				LogPT+= sprintf(LogPT, ")\n    Response content searching message. (");
				break;
			}

			// Send the search messages to neighbors.
			LogPT+= sprintf(LogPT, ")\n    Transit content searching message. (NeighborPeerIDs =");

			// Transit this message to neighbors.
			CWorkQueue WorkQueue;
			POSITION pos = DstPeerInfo->NeighborPeerIDMap.GetStartPosition();
			while(pos != NULL)
			{
				unsigned int NeighborPeerID;
				void *Temp;
				DstPeerInfo->NeighborPeerIDMap.GetNextAssoc(pos, NeighborPeerID, Temp);

				if(NeighborPeerID != SrcPeerID && NeighborPeerID != Message->FromPeerID && Message->FloodPath.Find(DstPeerID) == NULL)
				{
					CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, NeighborPeerID);
					WorkSendMessage->Message = new CMessage(Message->MessageID);
					WorkSendMessage->Message->SetSearchContent(Message->FromPeerID, Message->ContentID, &Message->FloodPath, DstPeerID);
					WorkQueue.QueueAtTail(WorkSendMessage);

					LogPT+= sprintf(LogPT, " %u", NeighborPeerID);
				}
			}
			Sim->InsertWork(Sim->Step, &WorkQueue, true);
			WorkQueue.RemoveAll();
		}
		break;
	}

	LogPT+= sprintf(LogPT, ")\n");
}
