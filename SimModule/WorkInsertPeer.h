#pragma once

#include "WorkBase.h"

class CWorkInsertPeer : public CWorkBase
{
public:
	CWorkInsertPeer(CSimulatorIn *Simulator);
	~CWorkInsertPeer();

	virtual	void								Simulate(char *Log, char *Dump);
};
