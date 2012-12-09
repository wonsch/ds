#include "stdafx.h"
#include "PeerInfo.h"

CPeerInfo::CPeerInfo(unsigned int PeerID)
{
	this->PeerID = PeerID;
	NewMessageID = 0;
}

CPeerInfo::~CPeerInfo()
{
	// Delete all PeerIDMessageIDs.
	POSITION pos = PeerIDMessageIDMap.GetStartPosition();
	while(pos != NULL)
	{
		unsigned int PeerID;
		CAtlMap<unsigned int, void*> *MessageIDMap;
		PeerIDMessageIDMap.GetNextAssoc(pos, PeerID, MessageIDMap);
		delete MessageIDMap;
	}

	// Delete all ContentIDPeerIDs.
	pos = ContentIDPeerID.GetStartPosition();
	while(pos != NULL)
	{
		unsigned int ContentID;
		CAtlList<unsigned int> *PeerIDList;
		ContentIDPeerID.GetNextAssoc(pos, ContentID, PeerIDList);
		delete PeerIDList;
	}
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

bool CPeerInfo::TryInsertPeerIDMessageID(unsigned int PeerID, unsigned MessageID)
{
	CAtlMap<unsigned int, void*> *MessageIDMap;
	if(PeerIDMessageIDMap.Lookup(PeerID, MessageIDMap) == false)
	{
		MessageIDMap = new CAtlMap<unsigned int, void*>();
		PeerIDMessageIDMap.SetAt(PeerID, MessageIDMap);
	}

	void *Temp;
	if(MessageIDMap->Lookup(MessageID, Temp) == true) return false;
	else
	{
		MessageIDMap->SetAt(MessageID, NULL);
		return true;
	}
}

CAtlList<unsigned int> *CPeerInfo::FindContentIDPeerIDList(unsigned int ContentID)
{
	// Find a peer which has the content from cache table.
	CAtlList<unsigned int> *PeerIDList;
	if(ContentIDPeerID.Lookup(ContentID, PeerIDList) == false) return NULL;
	else return PeerIDList;
}

void CPeerInfo::InsertContentIDPeerID(unsigned int ContentID, unsigned int PeerID)
{
	// Insert a peer which has the content into cache table.
	CAtlList<unsigned int> *PeerIDList;
	if(ContentIDPeerID.Lookup(ContentID, PeerIDList) == NULL)
	{
		PeerIDList = new CAtlList<unsigned int>();
		ContentIDPeerID.SetAt(ContentID, PeerIDList);
	}
	PeerIDList->AddTail(PeerID);
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
