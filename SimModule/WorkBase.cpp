#include "stdafx.h"
#include "WorkBase.h"

CWorkBase::CWorkBase()
{
	Sim = NULL;
	NextLink = NULL;
}

CWorkBase::CWorkBase(CSimulator *Simulator)
{
	Sim = Simulator;
	NextLink = NULL;
}

CWorkBase::~CWorkBase()
{
}

void CWorkBase::Simulate(char *Log)
{
	// This function will never be called.
	assert(false);
}
