#include "stdafx.h"
#include "WorkSendMessage.h"
#include "WorkRecvMessage.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

CWorkSendMessage::CWorkSendMessage(CSimulator *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID) : CWorkMessageBase(Simulator, SrcPeerID, DstPeerID)
{
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
			LogPT+= sprintf(LogPT, ", Message = SearchContent, FromPeerID = %u, ContentID = %08X", Message->FromPeerID, Message->ContentID);
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
