#pragma once

enum EMessage
{
	EMESSAGE_EMPTY = 0,
	EMESSAGE_NOTIFY_NULL,
	EMESSAGE_SEARCH_CONTENT,
};

class CMessage
{
public:
	CMessage(unsigned int MessageID);
	~CMessage();

	void										SetNotifyNull();
	void										SetSearchContent(unsigned int FromPeerID, unsigned int ContentID);

	EMessage									Message;

	unsigned int								MessageID;
	unsigned int								FromPeerID;
	unsigned int								ContentID;
};
