#include "stdafx.h"
#include "PeerInfo.h"

CPeerInfo::CPeerInfo(unsigned int PeerID)
{
	this->PeerID = PeerID;
	NewMessageID = 0;
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

void CPeerInfo::InsertContentInfo(unsigned int ContentID, CContentInfo *ContentInfo)
{
	// Insert a content.
	ContentInfoMap.SetAt(ContentID, ContentInfo);
}

int CPeerInfo::GerProperties(char *String)
{
	char *PT = String;
	PT+= sprintf(PT, "(PeerID = %u, NeighborPeerIDs =", PeerID);

	// Neighbor peers.
	POSITION pos = NeighborPeerIDMap.GetStartPosition();
	while(pos != NULL)
	{
		PT+= sprintf(PT, " %u", NeighborPeerIDMap.GetKeyAt(pos));
		NeighborPeerIDMap.GetNext(pos);
	}

	// Contents.
	PT+= sprintf(PT, ", ContentIDs =", PeerID);
	pos = ContentInfoMap.GetStartPosition();
	while(pos != NULL)
	{
		PT+= sprintf(PT, " %08X", ContentInfoMap.GetKeyAt(pos));
		ContentInfoMap.GetNext(pos);
	}

	PT+= sprintf(PT, ")");

	return PT - String;
}

int CPeerInfo::GetNeighborPeerIDs(char *String)
{
	char *PT = String;

	bool IsFirst = true;
	POSITION pos = NeighborPeerIDMap.GetStartPosition();
	while(pos != NULL)
	{
		unsigned int NeighborPeerID;
		void *Temp;
		NeighborPeerIDMap.GetNextAssoc(pos, NeighborPeerID, Temp);

		PT+= sprintf(PT, "%s%u", (IsFirst ? "" : " "), NeighborPeerID);
		IsFirst = false;
	}

	return PT - String;
}
