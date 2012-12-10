#include "stdafx.h"
#include "WorkSendMessage.h"
#include "WorkRecvMessage.h"
#include "PeerInfo.h"
#include "ContentInfo.h"

CWorkSendMessage::CWorkSendMessage(CSimulatorIn *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID) : CWorkMessageBase(Simulator, SrcPeerID, DstPeerID)
{
	DontIncreaseWorkNumber = true;
}

CWorkSendMessage::~CWorkSendMessage()
{
}

void CWorkSendMessage::Simulate(char *Log, char *Dump)
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

			// Dump
			Dump+= sprintf(Dump, "ETYPE=SEND_NOTIFY_NULL\n");
			//Dump+= sprintf(Dump, "EDETAIL=\n");
			Dump+= sprintf(Dump, "EACTOR=%u\n", SrcPeerID);
			Dump+= sprintf(Dump, "ETARGET=%u\n", DstPeerID);
			Dump+= sprintf(Dump, "ENOTE=Peer %u sends a notify message to peer %u.\n", SrcPeerID, DstPeerID);
		}
		break;

	case EMESSAGE_SEARCH_CONTENT:
		// Search a content
		{
			char *DumpLogPT = LogPT;
			LogPT+= sprintf(LogPT, ", Message = SearchContent, FromPeerID = %u, ContentID = %08X, FloodPath =", Message->FromPeerID, Message->ContentID);
			for(POSITION pos = Message->FloodPath.GetHeadPosition();pos != NULL;) LogPT+= sprintf(LogPT, " %u", Message->FloodPath.GetNext(pos));

			// Dump
			Dump+= sprintf(Dump, "ETYPE=SEND_SEARCH_CONTENT\n");
			//Dump+= sprintf(Dump, "EDETAIL=\n");
			Dump+= sprintf(Dump, "EACTOR=%u\n", SrcPeerID);
			Dump+= sprintf(Dump, "ETARGET=%u\n", DstPeerID);
			Dump+= sprintf(Dump, "ENOTE=%s\n", DumpLogPT + 2);
		}
		break;

	case EMESSAGE_SEARCH_CONTENT_RESPONSE_SOURCE:
		// Response a searching content message to source.
		{
			char *DumpLogPT = LogPT;
			LogPT+= sprintf(LogPT, ", Message = SearchContentResponseSource, FromPeerID = %u, ContentID = %08X, Hops = %u", Message->FromPeerID, Message->ContentID, Message->Hops);

			// Dump
			Dump+= sprintf(Dump, "ETYPE=SEARCH_CONTENT_RESPONSE_SOURCE\n");
			//Dump+= sprintf(Dump, "EDETAIL=\n");
			Dump+= sprintf(Dump, "EACTOR=%u\n", SrcPeerID);
			Dump+= sprintf(Dump, "ETARGET=%u\n", DstPeerID);
			Dump+= sprintf(Dump, "ENOTE=%s\n", DumpLogPT + 2);
		}
		break;

	case EMESSAGE_SEARCH_CONTENT_RESPONSE_PATH:
		// Response a searching content message to peers which is on the flood path.
		{
			char *DumpLogPT = LogPT;
			LogPT+= sprintf(LogPT, ", Message = SearchContentResponsePath, FromPeerID = %u, ContentID = %08X, FloodPath =", Message->FromPeerID, Message->ContentID);
			for(POSITION pos = Message->FloodPath.GetHeadPosition();pos != NULL;) LogPT+= sprintf(LogPT, " %u", Message->FloodPath.GetNext(pos));

			// Dump
			Dump+= sprintf(Dump, "ETYPE=SEND_SEARCH_CONTENT_RESPONSE_PATH\n");
			//Dump+= sprintf(Dump, "EDETAIL=\n");
			Dump+= sprintf(Dump, "EACTOR=%u\n", SrcPeerID);
			Dump+= sprintf(Dump, "ETARGET=%u\n", DstPeerID);
			Dump+= sprintf(Dump, "ENOTE=%s\n", DumpLogPT + 2);
		}
		break;

	case EMESSAGE_ASK_GROUPING:
		{
			LogPT+= sprintf(LogPT, ", Message = AskGrouping");

			// Dump
			Dump+= sprintf(Dump, "ETYPE=SEND_ASK_GROUPING\n");
			//Dump+= sprintf(Dump, "EDETAIL=\n");
			Dump+= sprintf(Dump, "EACTOR=%u\n", SrcPeerID);
			Dump+= sprintf(Dump, "ETARGET=%u\n", DstPeerID);
			Dump+= sprintf(Dump, "ENOTE=\n");
		}
		break;

	case EMESSAGE_REJECT_GROUPING:
		{
			LogPT+= sprintf(LogPT, ", Message = RejectGrouping");
		}
		break;

	case EMESSAGE_ACCEPT_GROUPING:
		{
			LogPT+= sprintf(LogPT, ", Message = AcceptGrouping");
		}
		break;

	case EMESSAGE_NOTIFY_GROUPING:
		{
			LogPT+= sprintf(LogPT, ", Message = NotifyGrouping");
		}
		break;

	case EMESSAGE_TRY_ASKING:
		{
			LogPT+= sprintf(LogPT, ", Message = TryAsking");
		}
		break;

	case EMESSAGE_CONTENT_INFO_UPDATE:
		{
			LogPT+= sprintf(LogPT, ", Message = ContentInfoUpdate, TTL = %u", Message->TTL);
		}
		break;
	}

	LogPT+= sprintf(LogPT, ")\n");

	// Insert the message into next step.
	CWorkRecvMessage *WorkRecvMessage = new CWorkRecvMessage(Sim, SrcPeerID, DstPeerID);
	WorkRecvMessage->Message = Message;
	Message = NULL;
	Sim->InsertWork(Sim->Step + 1, WorkRecvMessage);

	Sim->StatisticsTotalMessageCount++;
}
