#pragma once

#include "WorkBase.h"
#include "Message.h"

class CWorkMessageBase : public CWorkBase
{
public:
	CWorkMessageBase(CSimulator *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID);
	~CWorkMessageBase();

	bool										IsSource(unsigned int PeerID) {return PeerID == SrcPeerID;}
	bool										IsDestination(unsigned int PeerID) {return PeerID == DstPeerID;}

	unsigned int								SrcPeerID;
	unsigned int								DstPeerID;

	CMessage*									Message;
};
