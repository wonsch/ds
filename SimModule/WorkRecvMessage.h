#pragma once

#include "WorkMessageBase.h"

class CWorkRecvMessage : public CWorkMessageBase
{
public:
	CWorkRecvMessage(CSimulator *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID);
	~CWorkRecvMessage();

	virtual	void								Simulate(char *Log);
};
