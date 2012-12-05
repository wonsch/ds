#include "stdafx.h"
#include "WorkQueue.h"

CWorkQueue::CWorkQueue()
{
	WorkTail = &WorkHead;
}

CWorkQueue::~CWorkQueue()
{
}

void CWorkQueue::Queue(CWorkBase *Work)
{
	WorkTail->NextWork = Work;
	WorkTail = WorkTail->NextWork;
}

CWorkBase *CWorkQueue::DeQueue()
{
	if(IsEmpty() == true) return NULL;

	CWorkBase *Result = WorkHead.NextWork;
	WorkHead.NextWork = WorkHead.NextWork->NextWork;

	if(WorkTail == Result) WorkTail = &WorkHead;

	return Result;
}
