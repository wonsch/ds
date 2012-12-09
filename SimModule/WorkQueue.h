#pragma once

#include "WorkBase.h"

class CWorkQueue
{
public:
	CWorkQueue();
	~CWorkQueue();

	bool										IsEmpty();
	void										RemoveAll();

	void										QueueAtHead(CWorkQueue *Work);
	void										QueueAtHead(CWorkBase *Work);
	void										QueueAtTail(CWorkQueue *Work);
	void										QueueAtTail(CWorkBase *Work);
	bool										DeQueue(CWorkBase **Work, unsigned int *InNumber, unsigned int *ExNumber);

private:
	unsigned int								InNumber;
	unsigned int								ExNumber;
	CWorkBase									WorkHead;
	CWorkBase*									WorkTail;
};
