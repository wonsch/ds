#pragma once

#include "WorkBase.h"

class CWorkSearchContent : public CWorkBase
{
public:
	CWorkSearchContent(CSimulator *Simulator, unsigned int PeerID, unsigned int ContentID);
	~CWorkSearchContent();

	virtual	void								Simulate(char *Log);

	unsigned int								PeerID;
	unsigned int								ContentID;
};
