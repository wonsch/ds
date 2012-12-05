#pragma once

#include "WorkBase.h"

class CWorkQueue
{
public:
	CWorkQueue();
	~CWorkQueue();

	void										Queue(CWorkBase *Work);
	CWorkBase*									DeQueue();

	bool										IsEmpty() {return WorkHead.NextWork == NULL;}

private:
	CWorkBase									WorkHead;
	CWorkBase*									WorkTail;
};
