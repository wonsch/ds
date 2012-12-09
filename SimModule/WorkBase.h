#pragma once

#include "Simulator.h"

class CWorkBase
{
public:
	CWorkBase();
	CWorkBase(CSimulator *Simulator);
	virtual ~CWorkBase();

	virtual	void								Simulate(char *Log, char *Dump);

	CWorkBase*									NextLink;
	CSimulator*									Sim;
	bool										DontIncreaseWorkNumber;
};
