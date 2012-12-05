#pragma once

class CPeerInfo
{
public:
	CPeerInfo(unsigned int PeerID);
	~CPeerInfo();

	unsigned int								GetPeerID() {return PeerID;}

	bool										IsNeighborPeer(unsigned int PeerID);
	void										InsertNeighborPeerID(unsigned int PeerID);

	int											GerProperties(char *String);

private:
	unsigned int								PeerID;
	CAtlMap<unsigned int, void*>				NeighborPeerIDMap;
};
