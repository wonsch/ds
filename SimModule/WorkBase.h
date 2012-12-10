#pragma once

#include "SimulatorIn.h"

class CWorkBase
{
public:
	CWorkBase();
	CWorkBase(CSimulatorIn *Simulator);
	virtual ~CWorkBase();

	virtual	void								Simulate(char *Log, char *Dump);

	CWorkBase*									NextLink;
	CSimulatorIn*								Sim;
	bool										DontIncreaseWorkNumber;
};
