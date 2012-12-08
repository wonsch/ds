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

	unsigned int								GetNewMessageID() {return ++NewMessageID;}

	int											GerProperties(char *String);
	int											GetNeighborPeerIDs(char *String);

	unsigned int								PeerID;
	CAtlMap<unsigned int, void*>				NeighborPeerIDMap; // Map<PeerID, NULL>
	CAtlMap<unsigned int, CContentInfo*>		ContentInfoMap; // Map<ContentID, ContentInfo>
	CAtlMap<unsigned int, CAtlMap<unsigned int, void*>*> PeerIDMessageIDMap; // Map<PeerID, Map<MessageID, NULL>>

	unsigned int								NewMessageID;
};
