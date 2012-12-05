#include "stdafx.h"
#include "WorkQueue.h"

CWorkQueue::CWorkQueue()
{
	Number = 0;
	WorkTail = &WorkHead;
}

CWorkQueue::~CWorkQueue()
{
	CWorkBase *PT = WorkHead.NextLink, *Temp;
	while(PT != NULL)
	{
		Temp = PT;
		PT = PT->NextLink;
		delete Temp;
	}
}

bool CWorkQueue::IsEmpty()
{
	return WorkHead.NextLink == NULL;
}

void CWorkQueue::Queue(CWorkBase *Work)
{
	WorkTail->NextLink = Work;
	WorkTail = WorkTail->NextLink;
}

bool CWorkQueue::DeQueue(CWorkBase **Work, unsigned int *Number)
{
	if(IsEmpty() == true) return false;

	this->Number++;

	if(Work != NULL) *Work = WorkHead.NextLink;
	if(Number != NULL) *Number = this->Number;

	if(WorkTail == WorkHead.NextLink) WorkTail = &WorkHead;
	WorkHead.NextLink = WorkHead.NextLink->NextLink;

	return true;
}
