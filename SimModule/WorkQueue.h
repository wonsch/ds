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
	bool										DeQueue(CWorkBase **Work, unsigned int *Number);

private:
	unsigned int								Number;
	CWorkBase									WorkHead;
	CWorkBase*									WorkTail;
};
