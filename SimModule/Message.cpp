#include "stdafx.h"
#include "Message.h"

char *MesssageNames[] =
{
	"Empty",
	"NotifyNull",
	"SearchContent",
	"SearchContentResponseSource",
	"SearchContentResponsePath",
	"AskGrouping",
	"RejectGrouping",
	"AcceptGrouping",
	"NotifyGrouping",
	"TryAsking",
	"ContentInfoUdpate",
};

CMessage::CMessage(unsigned int MessageID)
{
	Message = EMESSAGE_EMPTY;

	this->MessageID = MessageID;

}

CMessage::~CMessage()
{
}

void CMessage::SetNotifyNull()
{
	Message = EMESSAGE_NOTIFY_NULL;
}
/*========================JIN==================================*/

void CMessage::SetContentInfoUpdate(unsigned int PeerID, unsigned int ContentID, int ttl)
{
	Message = EMESSAGE_CONTENT_INFO_UPDATE;
	this->FromPeerID = PeerID;
	this->ContentID = ContentID;
	this->TTL = ttl;
}
void CMessage::SetAskGroupAccept(unsigned int GroupID, unsigned int GroupMemberNumber, CAtlList<unsigned int> *pGroupMemberID)
{
	Message = EMESSAGE_ACCEPT_GROUPING;
	
	this->GroupID = GroupID;
	this->GroupMemberNumber = GroupMemberNumber;
	GroupMemberID.RemoveAll();

	if(pGroupMemberID != NULL) 
		GroupMemberID.AddTailList(pGroupMemberID);

}
void CMessage::SetNotifyGrouping()
{
	Message = EMESSAGE_NOTIFY_GROUPING;
}

void CMessage::SetAskGrouping()
{
	Message = EMESSAGE_ASK_GROUPING;
}

void CMessage::SetAskGroupReject()
{
	Message = EMESSAGE_REJECT_GROUPING;
}

/*========================JIN==================================*/
void CMessage::SetTryAsking()
{
	Message = EMESSAGE_TRY_ASKING;
}

void CMessage::SetSearchContent(unsigned int FromPeerID, unsigned int ContentID, CAtlList<unsigned int> *PrevFloodPath, unsigned int PeerIDToInsertFloodPath)
{
	Message = EMESSAGE_SEARCH_CONTENT;

	this->FromPeerID = FromPeerID;
	this->ContentID = ContentID;

	FloodPath.RemoveAll();
	if(PrevFloodPath != NULL) FloodPath.AddTailList(PrevFloodPath);
	FloodPath.AddTail(PeerIDToInsertFloodPath);
}

void CMessage::SetSearchContentResponseSource(unsigned int FromPeerID, unsigned int ContentID, unsigned int Hops, EResponseFrom ResponseFrom)
{
	Message = EMESSAGE_SEARCH_CONTENT_RESPONSE_SOURCE;

	this->FromPeerID = FromPeerID;
	this->ContentID = ContentID;
	this->ResponseFrom = ResponseFrom;
	this->Hops = Hops;
}

void CMessage::SetSearchContentResponsePath(unsigned int FromPeerID, unsigned int ContentID, CAtlList<unsigned int> *PrevFloodPath, unsigned int PeerIDToDeleteFloodPath, EResponseFrom ResponseFrom)
{
	Message = EMESSAGE_SEARCH_CONTENT_RESPONSE_PATH;

	this->FromPeerID = FromPeerID;
	this->ContentID = ContentID;
	this->ResponseFrom = ResponseFrom;

	FloodPath.RemoveAll();
	if(PrevFloodPath != NULL) FloodPath.AddTailList(PrevFloodPath);
	if(FloodPath.IsEmpty() == false && FloodPath.GetTail() == PeerIDToDeleteFloodPath) FloodPath.RemoveTailNoReturn();
}
