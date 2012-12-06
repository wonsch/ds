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

void CMessage::SetSearchContent(unsigned int FromPeerID, unsigned int ContentID)
{
	Message = EMESSAGE_SEARCH_CONTENT;

	this->FromPeerID = FromPeerID;
	this->ContentID = ContentID;
}
