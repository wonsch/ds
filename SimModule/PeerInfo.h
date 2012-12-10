#pragma once

class CContentInfo;

class CPeerInfo
{
public:
	CPeerInfo(unsigned int PeerID);
	~CPeerInfo();

	bool										IsNeighborPeer(unsigned int PeerID);
	void										InsertNeighborPeerID(unsigned int PeerID);

	void										InsertContentInfo(unsigned int ContentID, CContentInfo *ContentInfo);

	bool										TryInsertPeerIDMessageID(unsigned int PeerID, unsigned MessageID);

	CAtlList<unsigned int>*						FindContentIDPeerIDList(unsigned int ContentID);
	void										InsertContentIDPeerID(unsigned int ContentID, unsigned int PeerID);

	unsigned int								GetNewMessageID() {return ++NewMessageID;}

	int											GerProperties(char *String);
	int											GetNeighborPeerIDs(char *String);

	unsigned int								PeerID;
	CAtlMap<unsigned int, void*>				NeighborPeerIDMap; // Map<PeerID, NULL>
	CAtlMap<unsigned int, CContentInfo*>		ContentInfoMap; // Map<ContentID, ContentInfo>
	CAtlMap<unsigned int, CAtlMap<unsigned int, void*>*> PeerIDMessageIDMap; // Map<PeerID, Map<MessageID, NULL>>
	CAtlMap<unsigned int, CAtlList<unsigned int>*> ContentIDPeerID; // Map<ContentID, Map<PeerID, NULL>>

	unsigned int								NewMessageID;

	unsigned int								MinumumSearchContentHops;

	/* JIN */
	unsigned int groupID;
	unsigned int currGroupMemberNumber;

	CAtlMap<unsigned int, void*> MyGroupPeerIDMap; // my group peer id
	CAtlMap<unsigned int, void*> OtherGroupPeerIDMap; // other group peer id
	

											 
	//if there is no same contnetID of the neighborPeerID, insert the info 
	bool										InserNeighborContentID(unsigned int NeighborPeerID, unsigned int ContentID); 
	/* JIN */
};
