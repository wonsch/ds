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

	int											GerProperties(char *String);
	int											GetNeighborPeerIDs(char *String);

	unsigned int								PeerID;
	CAtlMap<unsigned int, void*>				NeighborPeerIDMap; // Map<PeerID, NULL>
	CAtlMap<unsigned int, CContentInfo*>		ContentInfoMap; // Map<ContentID, ContentInfo>

	unsigned int								NewMessageID;
};
