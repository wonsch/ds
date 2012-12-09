#pragma once

#include "WorkBase.h"

class CWorkSearchContent : public CWorkBase
{
public:
	CWorkSearchContent(CSimulator *Simulator, unsigned int PeerID, unsigned int ContentID);
	~CWorkSearchContent();

	virtual	void								Simulate(char *Log, char *Dump);

	unsigned int								PeerID;
	unsigned int								ContentID;
};
