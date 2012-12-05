#pragma once

#include "WorkQueue.h"

class CSim
{
public:
	CSim(unsigned long RandomSeed = 0);
	~CSim();

private:
	CwRand										wRand;
	CWorkQueue									WorkQueue;
};
