#include "stdafx.h"
#include "WorkBase.h"

CWorkBase::CWorkBase()
{
	NextLink = NULL;
	Sim = NULL;
	DontIncreaseWorkNumber = false;
}

CWorkBase::CWorkBase(CSimulator *Simulator)
{
	NextLink = NULL;
	Sim = Simulator;
	DontIncreaseWorkNumber = false;
}

CWorkBase::~CWorkBase()
{
}

void CWorkBase::Simulate(char *Log)
{
	// This function will never be called.
	assert(false);
}
