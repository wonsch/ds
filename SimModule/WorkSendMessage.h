#pragma once

#include "WorkMessageBase.h"

class CWorkSendMessage : public CWorkMessageBase
{
public:
	CWorkSendMessage(CSimulator *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID);
	~CWorkSendMessage();

	virtual	void								Simulate(char *Log);
};
