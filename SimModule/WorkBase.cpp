#include "stdafx.h"
#include "WorkBase.h"

CWorkBase::CWorkBase()
{
	NextLink = NULL;
	Sim = NULL;
	DontIncreaseWorkNumber = false;
}

CWorkBase::CWorkBase(CSimulatorIn *Simulator)
{
	NextLink = NULL;
	Sim = Simulator;
	DontIncreaseWorkNumber = false;
}

CWorkBase::~CWorkBase()
{
}

void CWorkBase::Simulate(char *Log, char *Dump)
{
	// This function will never be called.
	assert(false);
}
