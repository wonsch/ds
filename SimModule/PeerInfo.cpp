#include "stdafx.h"
#include "PeerInfo.h"

CPeerInfo::CPeerInfo(unsigned int PeerID)
{
	this->PeerID = PeerID;
}

CPeerInfo::~CPeerInfo()
{
}

bool CPeerInfo::IsNeighborPeer(unsigned int PeerID)
{
	// Is neighbor peer?
	void *Value;
	return NeighborPeerIDMap.Lookup(PeerID, Value);
}

void CPeerInfo::InsertNeighborPeerID(unsigned int PeerID)
{
	// Insert a neighbor peer ID.
	NeighborPeerIDMap.SetAt(PeerID, NULL);
}

int CPeerInfo::GerProperties(char *String)
{
	char *PT = String;
	PT+= sprintf(PT, "(PeerID = %u, NeighborPeerIDs =", PeerID);

	POSITION pos = NeighborPeerIDMap.GetStartPosition();
	while(pos != NULL)
	{
		PT+= sprintf(PT, " %u", NeighborPeerIDMap.GetKeyAt(pos));
		NeighborPeerIDMap.GetNext(pos);
	}

	PT+= sprintf(PT, ")");

	return PT - String;
}
