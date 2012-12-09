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

void CWorkRecvMessage::Simulate(char *Log, char *Dump)
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
			LogPT+= MessageEmpty(LogPT, Dump);
			break;

		case EMESSAGE_NOTIFY_NULL:
			// Notify NULL
			LogPT+= MessageNotifyNull(LogPT, Dump);
			break;

		case EMESSAGE_SEARCH_CONTENT:
			// Search a content
			LogPT+= MessageSearchContent(LogPT, Dump);
			break;
		
		case EMESSAGE_SEARCH_CONTENT_RESPONSE_SOURCE:
			// Response a searching content message to source.
			LogPT+= MessageSearchContentResponseSource(LogPT, Dump);
			break;

		case EMESSAGE_SEARCH_CONTENT_RESPONSE_PATH:
			// Response a searching content message to peers which is on the flood path.
			LogPT+= MessageSearchContentResponsePath(LogPT, Dump);
			break;
// JIN==========================================================================
		case EMESSAGE_ASK_GROUPING:
			LogPT+=MessageAskGrouping(LogPT, Dump);
			// 1. �� �׷쿡 ���ο� �Ǿ ���ü� �ִ��� Ȯ�� (max_num , curr_num ��) 
			// 2. ���� x  reject_grouping msg ������
			// 3. ���� o  curr_member++; accept_grouping msg ������ (group id, curr_num, curr_group_member_list)
			// 4. �ڱⲨ���� ���ο� �Ǿ� �׷��ڷᱸ���� �ְ�, ���̹� �ڷᱸ������ �ֱ� 
			break;
		case EMESSAGE_REJECT_GROUPING:
			// �ϴ� �ڱ� ID �� �׷� ����
			LogPT +=MessageRejectGrouping(LogPT, Dump);
			break;
		case EMESSAGE_ACCEPT_GROUPING:
			// ���� ������ �ڽ��� �׷� ������ ����
			LogPT += MessageAcceptGrouping(LogPT, Dump);
			break;
		case EMESSAGE_NOTIFY_GROUPING:
			LogPT += MessageNotifyGrouping(LogPT, Dump);
			break;
		case EMESSAGE_TRY_ASKING:
			LogPT += MessageTryAsking(LogPT, Dump);
			break;
		case EMESSAGE_CONTENT_INFO_UPDATE:
			LogPT += MessageUpdateContentInfo(LogPT, Dump);
			break;

		}
	}

	LogPT+= sprintf(LogPT, ")\n");
}

/*====================jin=========================*/

int CWorkRecvMessage::MessageUpdateContentInfo(char *Log, char *Dump)
{
	char* LogPT = Log;
	LogPT+= sprintf(LogPT, ", Message = UpdateContentInfo");

	this->DstPeerInfo->InsertContentIDPeerID(Message->ContentID, Message->ContentPeerID);

	return LogPT - Log;
}

int CWorkRecvMessage::MessageTryAsking(char *Log, char *Dump)
{
	char* LogPT = Log;
	LogPT+= sprintf(LogPT, ", Message = TryAsking");

	CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, this->DstPeerInfo->PeerID, SrcPeerID);
	WorkSendMessage->DontIncreaseWorkNumber = true;
	WorkSendMessage->Message = new CMessage(this->DstPeerInfo->GetNewMessageID());
	WorkSendMessage->Message->SetAskGrouping();
	
	Sim->InsertWork(Sim->Step, WorkSendMessage, true);

	return LogPT - Log; 
}

int CWorkRecvMessage::MessageRejectGrouping(char *Log, char *Dump)
{
	char* LogPT = Log;
	LogPT+= sprintf(LogPT, ", Message = RejectGrouping");

	// �ڱ� ���� ���� 
	if ( this->DstPeerInfo->groupID == 0 )
	{
		this->DstPeerInfo->groupID = this->DstPeerInfo->PeerID;
		LogPT+= sprintf(LogPT, ")\n    Make a temporary group. (GroupID = %u, GroupPeerIDs = %u", this->DstPeerID, this->DstPeerID);
	}
	return LogPT - Log;
}
int CWorkRecvMessage::MessageNotifyGrouping(char *Log, char *Dump)
{
	char* LogPT = Log;

	this->DstPeerInfo->GroupPeerIDMap.SetAt(this->SrcPeerID, NULL);
	this->DstPeerInfo->currGroupMemberNumber = this->DstPeerInfo->GroupPeerIDMap.GetCount();
	//printf("\n===================NOTI ========================\n   src : %u , dst : %u \n===================NOTI ========================\n",this->SrcPeerID, this->DstPeerID);

	LogPT+= sprintf(LogPT, ", Message = NotifyGrouping, GroupID = %u, GroupCount = %u", DstPeerInfo->groupID, DstPeerInfo->currGroupMemberNumber);

	return LogPT - Log;
}
int CWorkRecvMessage::MessageAcceptGrouping(char *Log, char *Dump)
{
	char *LogPT = Log;
	LogPT+= sprintf(LogPT, ", Message = AcceptGrouping, GroupID = %u, GroupPeerIDs = %u", Message->GroupID, DstPeerID);

	CWorkQueue WorkQueue;
	CWorkSendMessage *WorkSendMessage;
	if ( this->DstPeerInfo->groupID == 0 || this->DstPeerInfo->groupID == this->DstPeerInfo->PeerID )
	{
		// �ڱ� ���� ���� 
		this->DstPeerInfo->groupID = this->Message->GroupID;
		// �ڱ� �׷�(���� ��� ����) ���鿡�� �˸� 
		for(POSITION pos = this->Message->GroupMemberID.GetHeadPosition(); pos != NULL; this->Message->GroupMemberID.GetNext(pos))
		{
			unsigned int GroupPeerID = this->Message->GroupMemberID.GetAt(pos);

			this->DstPeerInfo->GroupPeerIDMap.SetAt(GroupPeerID, NULL);
			this->DstPeerInfo->currGroupMemberNumber += 1;
			
			if(SrcPeerID != GroupPeerID)
			{
				WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, GroupPeerID);
				WorkSendMessage->Message = new CMessage(DstPeerInfo->GetNewMessageID());
				WorkSendMessage->Message->SetNotifyGrouping();
				WorkQueue.QueueAtTail(WorkSendMessage);
				
			}
			LogPT+= sprintf(LogPT, " %u", GroupPeerID);
		
			
		}
			
		Sim->InsertWork(Sim->Step, &WorkQueue, true);
		WorkQueue.RemoveAll();
		this->Message->GroupMemberID.RemoveAll();
	
	}
	return LogPT- Log;

}

int CWorkRecvMessage::MessageAskGrouping(char *Log, char *Dump)
{
	char *LogPT = Log;
	LogPT+= sprintf(LogPT, ", Message = AskGrouping, GroupID = %u, GroupPeerIDs =", DstPeerInfo->groupID);

	CWorkQueue WorkQueue;
	CWorkSendMessage *WorkSendMessage;
	WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, SrcPeerID);
	WorkSendMessage->Message = new CMessage(DstPeerInfo->GetNewMessageID());
	DstPeerInfo->InsertNeighborPeerID(SrcPeerID);
	// 1. �� �׷쿡 ���ο� �Ǿ ���ü� �ִ��� Ȯ�� (max_num , curr_num ��) 
	
	if(this->DstPeerInfo->groupID == 0) 
	{
		WorkSendMessage->Message->SetTryAsking();
		WorkQueue.QueueAtTail(WorkSendMessage);
		Sim->InsertWork(Sim->Step, &WorkQueue, true);
		WorkQueue.RemoveAll();
		return LogPT-Log;
	}
	
	if(DstPeerInfo->currGroupMemberNumber < Sim->GroupMaxNumber )
	{
		// 3. ���� o  curr_member++; accept_grouping msg ������ (group id, curr_num, curr_group_member_list)
		// 4. �ڱⲨ���� ���ο� �Ǿ� �׷��ڷᱸ���� �ְ�, ���̹� �ڷᱸ������ �ֱ�
		CAtlList<unsigned int> groupList;
		groupList.AddTail(DstPeerInfo->PeerID);

		POSITION pos = DstPeerInfo->GroupPeerIDMap.GetStartPosition();
		//printf("\n================HAVE GROUP MEMBER ID ==================\n");
		//printf(" src id : %u , dst id : %u \n",this->SrcPeerID, this->DstPeerID);
		while(pos != NULL)
		{
			unsigned int GroupPeerID;
			void *Temp;
			DstPeerInfo->GroupPeerIDMap.GetNextAssoc(pos,GroupPeerID,Temp);
			groupList.AddTail(GroupPeerID);
			//printf("  group id : % u \n",GroupPeerID);
			
		}
		//printf("\n================HAVE GROUP MEMBER ID ==================\n");  
		DstPeerInfo->GroupPeerIDMap.SetAt(this->SrcPeerID, NULL);  //my�׷쿡 �ֱ�
		DstPeerInfo->currGroupMemberNumber += 1;

		WorkSendMessage->Message->SetAskGroupAccept(DstPeerInfo->groupID, DstPeerInfo->currGroupMemberNumber, &groupList);
		WorkQueue.QueueAtTail(WorkSendMessage);
	}
	else
	{
		// 2. ���� x  reject_grouping msg ������
		WorkSendMessage->Message->SetAskGroupReject();  //other�׷쿡 �ֱ�
		WorkQueue.QueueAtTail(WorkSendMessage);
	}

	LogPT+= sprintf(LogPT, " %u", DstPeerInfo->PeerID);
	POSITION pos = DstPeerInfo->GroupPeerIDMap.GetStartPosition();
	while(pos != NULL)
	{
		unsigned int GroupPeerID;
		void *Temp;
		DstPeerInfo->GroupPeerIDMap.GetNextAssoc(pos, GroupPeerID, Temp);
		LogPT+= sprintf(LogPT, " %u", GroupPeerID);
	}

	//DstPeerInfo->NeighborPeerIDMap.SetAt(this->SrcPeerID,NULL); // total neighbor �׷쿡 �ֱ�

	Sim->InsertWork(Sim->Step, &WorkQueue, true);
	WorkQueue.RemoveAll();

	return LogPT - Log;

}


/*====================jin END=========================*/
int CWorkRecvMessage::MessageEmpty(char *Log, char *Dump)
{
	return 0;
}

int CWorkRecvMessage::MessageNotifyNull(char *Log, char *Dump)
{
	char *LogPT = Log;

	// Insert a peer into the neighbor.
	DstPeerInfo->InsertNeighborPeerID(SrcPeerID);
	LogPT+= sprintf(LogPT, ", Message = NotifyNull)\n");
	LogPT+= sprintf(LogPT, "    PeerID %u inserted the PeerID %u into its neighbor list. (NeighborPeerIDs = ", DstPeerID, SrcPeerID);
	LogPT+= DstPeerInfo->GetNeighborPeerIDs(LogPT);

	return LogPT - Log;
}

int CWorkRecvMessage::MessageSearchContent(char *Log, char *Dump)
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

int CWorkRecvMessage::MessageSearchContentResponseSource(char *Log, char *Dump)
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

int CWorkRecvMessage::MessageSearchContentResponsePath(char *Log, char *Dump)
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
	if(Sim->CacheMode == MODE_CACHE_ON) 
	{	
		DstPeerInfo->InsertContentIDPeerID(Message->ContentID, Message->FromPeerID);
	// TODO (JIN): �ٸ� �׷��� ��忡�� �ڽ��� ������ ������ location ������ ���� -> �� ���� �����ϰ� ������ 
	
		CWorkQueue WorkQueue;
		POSITION pos2 = DstPeerInfo->GroupPeerIDMap.GetStartPosition();
		bool diff = true;

		while(pos2 != NULL)
		{
			unsigned int GroupPeerID;
			void *temp2;
			DstPeerInfo->GroupPeerIDMap.GetNextAssoc(pos2, GroupPeerID, temp2);
			POSITION pos = DstPeerInfo->NeighborPeerIDMap.GetStartPosition();
			while(pos != NULL)
			{
				unsigned int NeighborPeerID;
				void *Temp;
				DstPeerInfo->NeighborPeerIDMap.GetNextAssoc(pos, NeighborPeerID, Temp);
				if(GroupPeerID == NeighborPeerID)
				{
					diff = false;
				}

			}
			if(diff)
			{
				CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, GroupPeerID);
				WorkSendMessage->Message = new CMessage(Message->MessageID);
				WorkSendMessage->Message->SetContentInfoUpdate(Message->ContentID, Message->FromPeerID);
				WorkQueue.QueueAtTail(WorkSendMessage);
				LogPT+= sprintf(LogPT, " %u", GroupPeerID);


			}
			else
			{
				diff = true;
			}
		}
		Sim->InsertWork(Sim->Step, &WorkQueue, true);
		WorkQueue.RemoveAll();


	}
	// Get a next backward peer ID.
	LastPeerID = Message->FloodPath.GetTail();

	CWorkSendMessage *WorkSendMessage = new CWorkSendMessage(Sim, DstPeerID, LastPeerID);
	WorkSendMessage->Message = new CMessage(DstPeerInfo->GetNewMessageID());
	WorkSendMessage->Message->SetSearchContentResponsePath(Message->FromPeerID, Message->ContentID, &Message->FloodPath, DstPeerInfo->PeerID, Message->ResponseFrom);
	Sim->InsertWork(Sim->Step, WorkSendMessage, true);

	return LogPT - Log;
}
