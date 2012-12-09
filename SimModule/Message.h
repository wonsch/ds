#pragma once

enum EMessage
{
	EMESSAGE_EMPTY = 0,
	EMESSAGE_NOTIFY_NULL,
	EMESSAGE_SEARCH_CONTENT,
	EMESSAGE_SEARCH_CONTENT_RESPONSE_SOURCE,
	EMESSAGE_SEARCH_CONTENT_RESPONSE_PATH,
	EMESSAGE_ASK_GROUPING,
	EMSSAGE_REJECT_GROUPING,
	EMSSAGE_ACCEPT_GROUPING,
	EMESSAGE_NOTIFY_GROUPING,
};

enum EResponseFrom
{
	ERESPONSE_FROM_SOURCE = 0,
	ERESPONSE_FROM_CACHE,
};

class CMessage
{
public:
	CMessage(unsigned int MessageID);
	~CMessage();

	void										SetNotifyNull();
	void										SetSearchContent(unsigned int FromPeerID, unsigned int ContentID, CAtlList<unsigned int> *PrevFloodPath, unsigned int PeerIDToInsertFloodPath);
	void										SetSearchContentResponseSource(unsigned int FromPeerID, unsigned int ContentID, unsigned int Hops, EResponseFrom ResponseFrom);
	void										SetSearchContentResponsePath(unsigned int FromPeerID, unsigned int ContentID, CAtlList<unsigned int> *PrevFloodPath, unsigned int PeerIDToDeleteFloodPath, EResponseFrom ResponseFrom);

	EMessage									Message;

	unsigned int								MessageID;
	unsigned int								FromPeerID;
	unsigned int								ContentID;
	EResponseFrom								ResponseFrom;

	unsigned int								Hops;
	CAtlList<unsigned int>						FloodPath;

	/*========================JIN==================================*/
	void										SetAskGroupAccept(unsigned int GroupID, unsigned int GroupMemberNumber, CAtlList<unsigned int>* pGroupMemberID);
	void										SetNotifyGrouping();
	void										SetAskGrouping();
	void										SetAskGroupReject();
	unsigned int								GroupID;
	unsigned int								GroupMemberNumber;
	CAtlList<unsigned int>						GroupMemberID;

	/*========================JIN==================================*/

};
