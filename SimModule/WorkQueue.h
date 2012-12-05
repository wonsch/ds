#pragma once

#include "WorkBase.h"

class CWorkQueue
{
public:
	CWorkQueue();
	~CWorkQueue();

	bool										IsEmpty();

	void										Queue(CWorkBase *Work);
	bool										DeQueue(CWorkBase **Work, unsigned int *Number);

private:
	unsigned int								Number;
	CWorkBase									WorkHead;
	CWorkBase*									WorkTail;
};
