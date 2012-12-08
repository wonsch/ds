#include "stdafx.h"
#include "Message.h"

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

void CMessage::SetSearchContent(unsigned int FromPeerID, unsigned int ContentID, CAtlList<unsigned int> *PrevFloodPath, unsigned int PeerIDToInsertFloodPath)
{
	Message = EMESSAGE_SEARCH_CONTENT;

	this->FromPeerID = FromPeerID;
	this->ContentID = ContentID;

	FloodPath.RemoveAll();
	if(PrevFloodPath != NULL) FloodPath.AddTailList(PrevFloodPath);
	FloodPath.AddTail(PeerIDToInsertFloodPath);
}

void CMessage::SetSearchContentResponseSource(unsigned int FromPeerID, unsigned int ContentID)
{
	Message = EMESSAGE_SEARCH_CONTENT_RESPONSE_SOURCE;

	this->FromPeerID = FromPeerID;
	this->ContentID = ContentID;
}

void CMessage::SetSearchContentResponsePath(unsigned int FromPeerID, unsigned int ContentID, CAtlList<unsigned int> *PrevFloodPath, unsigned int PeerIDToDeleteFloodPath)
{
	Message = EMESSAGE_SEARCH_CONTENT_RESPONSE_PATH;

	this->FromPeerID = FromPeerID;
	this->ContentID = ContentID;

	FloodPath.RemoveAll();
	if(PrevFloodPath != NULL) FloodPath.AddTailList(PrevFloodPath);
	if(FloodPath.IsEmpty() == false && FloodPath.GetTail() == PeerIDToDeleteFloodPath) FloodPath.RemoveTailNoReturn();
}
