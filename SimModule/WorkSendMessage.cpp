#include "stdafx.h"
#include "WorkSendMessage.h"
#include "WorkRecvMessage.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

CWorkSendMessage::CWorkSendMessage(CSimulator *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID) : CWorkMessageBase(Simulator, SrcPeerID, DstPeerID)
{
	DontIncreaseWorkNumber = true;
}

CWorkSendMessage::~CWorkSendMessage()
{
}

void CWorkSendMessage::Simulate(char *Log)
{
	assert(Message != NULL);

	char *LogPT = Log;
	LogPT+= sprintf(LogPT, "    Send a message. (SrcPeerID = %u, DstPeerID = %u, MessageID = %u", SrcPeerID, DstPeerID, Message->MessageID);

	switch(Message->Message)
	{
	case EMESSAGE_EMPTY:
		// Empty
		break;

	case EMESSAGE_NOTIFY_NULL:
		// Notify NULL
		{
			LogPT+= sprintf(LogPT, ", Message = NotifyNull");
		}
		break;

	case EMESSAGE_SEARCH_CONTENT:
		// Search a content
		{
			LogPT+= sprintf(LogPT, ", Message = SearchContent, FromPeerID = %u, ContentID = %08X, FloodPath =", Message->FromPeerID, Message->ContentID);
			for(POSITION pos = Message->FloodPath.GetHeadPosition();pos != NULL;) LogPT+= sprintf(LogPT, " %u", Message->FloodPath.GetNext(pos));
		}
		break;

	case EMESSAGE_SEARCH_CONTENT_RESPONSE_SOURCE:
		// Response a searching content message to source.
		{
			LogPT+= sprintf(LogPT, ", Message = SearchContentResponseSource, FromPeerID = %u, ContentID = %08X", Message->FromPeerID, Message->ContentID);
		}
		break;

	case EMESSAGE_SEARCH_CONTENT_RESPONSE_PATH:
		// Response a searching content message to peers which is on the flood path.
		{
			LogPT+= sprintf(LogPT, ", Message = SearchContentResponsePath, FromPeerID = %u, ContentID = %08X, FloodPath =", Message->FromPeerID, Message->ContentID);
			for(POSITION pos = Message->FloodPath.GetHeadPosition();pos != NULL;) LogPT+= sprintf(LogPT, " %u", Message->FloodPath.GetNext(pos));
		}
		break;
	case EMESSAGE_ASK_GROUPING:
		{

			LogPT+= sprintf(LogPT, ", Message = EMESSAGE_ASK_GROUPING, FromPeerID = %u, ContentID = %08X, FloodPath =", Message->FromPeerID, Message->ContentID);
		}
		break;

	case EMSSAGE_REJECT_GROUPING:
		{
			LogPT+= sprintf(LogPT, ", Message = EMSSAGE_REJECT_GROUPING, FromPeerID = %u, ContentID = %08X, FloodPath =", Message->FromPeerID, Message->ContentID);
		}
		break;
	case EMSSAGE_ACCEPT_GROUPING:
		{
			LogPT+= sprintf(LogPT, ", Message = EMSSAGE_ACCEPT_GROUPING, FromPeerID = %u, ContentID = %08X, FloodPath =", Message->FromPeerID, Message->ContentID);
			for(POSITION pos = Message->GroupMemberID.GetHeadPosition();pos != NULL;) LogPT+= sprintf(LogPT, " %u", Message->GroupMemberID.GetNext(pos));
		}
		break;
	}

	LogPT+= sprintf(LogPT, ")\n");

	// Insert the message into next step.
	CWorkRecvMessage *WorkRecvMessage = new CWorkRecvMessage(Sim, SrcPeerID, DstPeerID);
	WorkRecvMessage->Message = Message;
	Message = NULL;
	Sim->InsertWork(Sim->Step + 1, WorkRecvMessage);
}
