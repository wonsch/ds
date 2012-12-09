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

		case EMESSAGE_ASK_GROUPING:
			LogPT+=MessageAskGrouping(LogPT);
			// 1. 내 그룹에 새로운 피어가 들어올수 있는지 확인 (max_num , curr_num 비교) 
			// 2. 가능 x  reject_grouping msg 보내기
			// 3. 가능 o  curr_member++; accept_grouping msg 보내기 (group id, curr_num, curr_group_member_list)
			// 4. 자기꺼에서 새로운 피어 그룹자료구조에 넣고, 네이버 자료구조에도 넣기 
			break;
		case EMESSAGE_REJECT_GROUPING:
			// 일단 자기 ID 로 그룹 생성
			LogPT +=MessageRejectGrouping(LogPT);
			break;
		case EMESSAGE_ACCEPT_GROUPING:
			// 받은 정보로 자신의 그룹 정보를 갱신
			LogPT += MessageAcceptGrouping(LogPT);
			break;
		}
	}

	LogPT+= sprintf(LogPT, ")\n");
}

/*====================jin=========================*/
int CWorkRecvMessage::MessageRejectGrouping(char* Log)
{
	return 0;
}
int CWorkRecvMessage::MessageAcceptGrouping(char* Log)
{
	char *LogPT = Log;
	// 자기 정보 갱신 
	// 자기 그룹(승인 노드 뺴고) 노드에게 알림 

	return LogPT- Log;

}

int CWorkRecvMessage::MessageAskGrouping(char *Log)
{
	char *LogPT = Log;
	CWorkQueue WorkQueue;
	CWorkSendMessage *WorkSendMessage;
	WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, SrcPeerID);
	WorkSendMessage->Message = new CMessage(DstPeerInfo->GetNewMessageID());
	// 1. 내 그룹에 새로운 피어가 들어올수 있는지 확인 (max_num , curr_num 비교) 
	
	
	
	if(DstPeerInfo->currGroupMemberNumber < Sim->GroupMaxNumber )
	{
		// 3. 가능 o  curr_member++; accept_grouping msg 보내기 (group id, curr_num, curr_group_member_list)
		// 4. 자기꺼에서 새로운 피어 그룹자료구조에 넣고, 네이버 자료구조에도 넣기
		CAtlList<unsigned int> groupList;
		groupList.AddTail(DstPeerInfo->PeerID);

		POSITION pos = DstPeerInfo->GroupPeerIDMap.GetStartPosition();

		while(pos != NULL)
		{
			unsigned int GroupPeerID;
			void *Temp;
			DstPeerInfo->GroupPeerIDMap.GetNextAssoc(pos,GroupPeerID,Temp);
			groupList.AddTail(GroupPeerID);
			LogPT+= sprintf(LogPT, " %u", GroupPeerID);
		}
		
		  
		DstPeerInfo->GroupPeerIDMap.SetAt(this->SrcPeerID,NULL);  //my그룹에 넣기
		DstPeerInfo->currGroupMemberNumber += 1;

		WorkSendMessage->Message->SetAskGroupAccept(DstPeerInfo->groupID,DstPeerInfo->currGroupMemberNumber,&groupList);
		WorkQueue.QueueAtTail(WorkSendMessage);


	}
	else  // 그룹 꽉 찼음
	{
		// 2. 가능 x  reject_grouping msg 보내기
		WorkSendMessage->Message->SetAskGroupReject();  //other그룹에 넣기
		WorkQueue.QueueAtTail(WorkSendMessage);

		DstPeerInfo->OtherGroupPeerIDMap.SetAt(this->SrcPeerID, NULL);
		
	}

	DstPeerInfo->NeighborPeerIDMap.SetAt(this->SrcPeerID,NULL); // total neighbor 그룹에 넣기

	Sim->InsertWork(Sim->Step, &WorkQueue, true);
	WorkQueue.RemoveAll();

	return LogPT - Log;

}


/*====================jin=========================*/
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

	// Should this peer transit this message to its neighbors?
	// = Has this peer processed this message before?
	if(DstPeerInfo->TryInsertPeerIDMessageID(Message->FromPeerID, Message->MessageID) == false) return LogPT - Log;

	// Does this peer have the content?
	// or know other peers which have the content? (cache)
	int FoundMode = 0;
	unsigned int FoundPeerID;
	CContentInfo *ContentInfo;
	CAtlList<unsigned int> *PeerIDList;
	if(DstPeerInfo->ContentInfoMap.Lookup(Message->ContentID, ContentInfo) == true)
	{
		FoundMode = 1;
		FoundPeerID = DstPeerInfo->PeerID;
	}
	else if(Sim->CacheMode == MODE_CACHE_ON && (PeerIDList = DstPeerInfo->FindContentIDPeerIDList(Message->ContentID)) != NULL)
	{
		FoundMode = 2;
		FoundPeerID = PeerIDList->GetHead();
	}

	// If this peer have the content or know peers which have the content.
	if(FoundMode != 0)
	{
		EResponseFrom ResponseFrom = (FoundMode == 1 ? ERESPONSE_FROM_SOURCE : ERESPONSE_FROM_CACHE);
		CWorkQueue WorkQueue;

		// Response to a peer which made a original request.
		CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, Message->FromPeerID);
		WorkSendMessage->Message = new CMessage(DstPeerInfo->GetNewMessageID());
		WorkSendMessage->Message->SetSearchContentResponseSource(FoundPeerID, Message->ContentID, Message->FloodPath.GetCount(), ResponseFrom);
		WorkQueue.QueueAtTail(WorkSendMessage);

		// Response to a previous peer to transmit.
		WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, SrcPeerID);
		WorkSendMessage->Message = new CMessage(DstPeerInfo->GetNewMessageID());
		WorkSendMessage->Message->SetSearchContentResponsePath(DstPeerInfo->PeerID, Message->ContentID, &Message->FloodPath, DstPeerInfo->PeerID, ResponseFrom);
		WorkQueue.QueueAtTail(WorkSendMessage);

		Sim->InsertWork(Sim->Step, &WorkQueue, true);
		WorkQueue.RemoveAll();

		LogPT+= sprintf(LogPT, ")\n    Response content searching message. (From = ");
		if(ResponseFrom == ERESPONSE_FROM_SOURCE) LogPT+= sprintf(LogPT, "Source");
		else LogPT+= sprintf(LogPT, "Cache");

		CAtlString String;
		String.Format("Step %u : Peer %u has a content %08X. (FromPeerID = %u, Hops = %u, ResponseFrom = %s)\n", Sim->Step, DstPeerID, Message->ContentID, Message->FromPeerID, Message->FloodPath.GetCount(), (ResponseFrom == ERESPONSE_FROM_SOURCE ? "Source" : "Cache"));
		Sim->AttachLog(String);

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
	if(DstPeerInfo->MinumumSearchContentHops > Message->Hops)
	{
		DstPeerInfo->MinumumSearchContentHops = Message->Hops;
		Sim->StatisticsTotalSearchContentSuccessCount++;
		Sim->StatisticsTotalSearchContentHopCount+= Message->Hops;
	}

	CAtlString String;
	String.Format("Step %u : Peer %u found a content %08X. (SrcPeerID = %u, FromPeerID = %u, Hops = %u, ResponseFrom = %s).\n", Sim->Step, DstPeerID, Message->ContentID, SrcPeerID, Message->FromPeerID, Message->Hops, (Message->ResponseFrom == ERESPONSE_FROM_SOURCE ? "Source" : "Cache"));
	Sim->AttachLog(String);

	LogPT+= sprintf(LogPT, ", Message = SearchContentResponseSource, FromPeerID = %u, ContentID = %08X, Hops = %u", Message->FromPeerID, Message->ContentID, Message->Hops);

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

	// Insert into cache table.
	if(Sim->CacheMode == MODE_CACHE_ON) DstPeerInfo->InsertContentIDPeerID(Message->ContentID, Message->FromPeerID);

	// Get a next backward peer ID.
	LastPeerID = Message->FloodPath.GetTail();

	CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, LastPeerID);
	WorkSendMessage->Message = new CMessage(DstPeerInfo->GetNewMessageID());
	WorkSendMessage->Message->SetSearchContentResponsePath(Message->FromPeerID, Message->ContentID, &Message->FloodPath, DstPeerInfo->PeerID, Message->ResponseFrom);
	Sim->InsertWork(Sim->Step, WorkSendMessage, true);

	return LogPT - Log;
}
