#pragma once

#include "Simulator.h"

class CWorkBase
{
public:
	CWorkBase();
	CWorkBase(CSimulator *Simulator);
	virtual ~CWorkBase();

	virtual	void								Simulate(char *Log);

	CSimulator*									Sim;
	CWorkBase*									NextLink;
};
