#include "stdafx.h"
#include "WorkQueue.h"
#include "WorkSendMessage.h"
#include "WorkRecvMessage.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

CWorkRecvMessage::CWorkRecvMessage(CSimulator *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID) : CWorkMessageBase(Simulator, SrcPeerID, DstPeerID)
{
	DstPeerInfo = NULL;
}

CWorkRecvMessage::~CWorkRecvMessage()
{
}

void CWorkRecvMessage::Simulate(char *Log)
{
	assert(Message != NULL);

	char *LogPT = Log;
	LogPT+= sprintf(LogPT, "    Receive a message. (SrcPeerID = %u, DstPeerID = %u, MessageID = %u", SrcPeerID, DstPeerID, Message->MessageID);

	// Get the destination peer.
	if(Sim->PeerInfoMap.Lookup(DstPeerID, DstPeerInfo) == true)
	{
		switch(Message->Message)
		{
		case EMESSAGE_EMPTY:
			// Empty
			LogPT+= MessageEmpty(LogPT);
			break;

		case EMESSAGE_NOTIFY_NULL:
			// Notify NULL
			LogPT+= MessageNotifyNull(LogPT);
			break;

		case EMESSAGE_SEARCH_CONTENT:
			// Search a content
			LogPT+= MessageSearchContent(LogPT);
			break;
		
		case EMESSAGE_SEARCH_CONTENT_RESPONSE_SOURCE:
			// Response a searching content message to source.
			LogPT+= MessageSearchContentResponseSource(LogPT);
			break;

		case EMESSAGE_SEARCH_CONTENT_RESPONSE_PATH:
			// Response a searching content message to peers which is on the flood path.
			LogPT+= MessageSearchContentResponsePath(LogPT);
			break;
		}
	}

	LogPT+= sprintf(LogPT, ")\n");
}

int CWorkRecvMessage::MessageEmpty(char *Log)
{
	return 0;
}

int CWorkRecvMessage::MessageNotifyNull(char *Log)
{
	char *LogPT = Log;

	// Insert a peer into the neighbor.
	DstPeerInfo->InsertNeighborPeerID(SrcPeerID);
	LogPT+= sprintf(LogPT, ", Message = NotifyNull)\n");
	LogPT+= sprintf(LogPT, "    PeerID %u inserted the PeerID %u into its neighbor list. (NeighborPeerIDs = ", DstPeerID, SrcPeerID);
	LogPT+= DstPeerInfo->GetNeighborPeerIDs(LogPT);

	return LogPT - Log;
}

int CWorkRecvMessage::MessageSearchContent(char *Log)
{
	char *LogPT = Log;

	LogPT+= sprintf(LogPT, ", Message = SearchContent, FromPeerID = %u, ContentID = %08X, FloodPath =", Message->FromPeerID, Message->ContentID);
	for(POSITION pos = Message->FloodPath.GetHeadPosition();pos != NULL;) LogPT+= sprintf(LogPT, " %u", Message->FloodPath.GetNext(pos));

	// Insert a peer into the neighbor.
	DstPeerInfo->InsertNeighborPeerID(SrcPeerID);

	// Should the peer transit this message to its neighbors?
	// = Has the peer processed this message before?
	if(DstPeerInfo->TryInsertPeerIDMessageID(Message->FromPeerID, Message->MessageID) == false) return LogPT - Log;

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

		CAtlString String;
		String.Format("Step %u : Peer %u has a content %08X. (FromPeerID = %u, Hops = %u)\n", Sim->Step, DstPeerID, Message->ContentID, SrcPeerID, Message->FloodPath.GetCount());
		Sim->AttachLog(String);

		Sim->StatisticsTotalSearchContentHopCount+= Message->FloodPath.GetCount();

		return LogPT - Log;
	}

	// Send the search messages to neighbors.
	LogPT+= sprintf(LogPT, ")\n    Transit content searching message. (NeighborPeerIDs =");

	if(Message->FloodPath.GetCount() >= Sim->InitMaxFloodHopCount) return LogPT - Log;

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

	return LogPT - Log;
}

int CWorkRecvMessage::MessageSearchContentResponseSource(char *Log)
{
	char *LogPT = Log;

	// TODO: The peer received a response of searching content.
	Sim->StatisticsTotalSearchContentSuccessCount++;
	CAtlString String;
	String.Format("Step %u : Peer %u found a content %08X at peer %u.\n", Sim->Step, SrcPeerID, Message->ContentID, DstPeerID);
	Sim->AttachLog(String);

	LogPT+= sprintf(LogPT, ", Message = SearchContentResponseSource, FromPeerID = %u, ContentID = %08X", Message->FromPeerID, Message->ContentID);

	return LogPT - Log;
}

int CWorkRecvMessage::MessageSearchContentResponsePath(char *Log)
{
	char *LogPT = Log;

	// Transit a searching content response message
	LogPT+= sprintf(LogPT, ", Message = SearchContentResponsePath, FromPeerID = %u, ContentID = %08X, FloodPath =", Message->FromPeerID, Message->ContentID);
	for(POSITION pos = Message->FloodPath.GetHeadPosition();pos != NULL;) LogPT+= sprintf(LogPT, " %u", Message->FloodPath.GetNext(pos));

	if(Message->FloodPath.IsEmpty() == true) return LogPT - Log;
	unsigned int LastPeerID = Message->FloodPath.RemoveTail();
	if(LastPeerID != DstPeerID) return LogPT - Log;

	if(Message->FloodPath.IsEmpty() == true)
	{
		// TODO: The peer received a response of searching content.
		return LogPT - Log;
	}

	// Get a next backward peer ID.
	LastPeerID = Message->FloodPath.GetTail();

	CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, LastPeerID);
	WorkSendMessage->Message = new CMessage(DstPeerInfo->GetNewMessageID());
	WorkSendMessage->Message->SetSearchContentResponsePath(Message->FromPeerID, Message->ContentID, &Message->FloodPath, DstPeerInfo->PeerID);
	Sim->InsertWork(Sim->Step, WorkSendMessage, true);

	return LogPT - Log;
}
