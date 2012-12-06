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

void CWorkQueue::RemoveAll()
{
	WorkHead.NextLink = NULL;
	WorkTail = &WorkHead;
}

void CWorkQueue::QueueAtHead(CWorkQueue *WorkQueue)
{
	WorkQueue->WorkTail->NextLink = WorkHead.NextLink;
	WorkHead.NextLink = WorkQueue->WorkHead.NextLink;
	if(WorkTail == &WorkHead) WorkTail = WorkQueue->WorkTail;
}

void CWorkQueue::QueueAtHead(CWorkBase *Work)
{
	Work->NextLink = WorkHead.NextLink;
	WorkHead.NextLink = Work;
	if(WorkTail == &WorkHead) WorkTail = Work;
}

void CWorkQueue::QueueAtTail(CWorkQueue *Work)
{
	// Need to be tested.
	if(Work->IsEmpty() == true) return;
	WorkTail->NextLink = Work->WorkHead.NextLink;
	WorkTail = Work->WorkTail;
}

void CWorkQueue::QueueAtTail(CWorkBase *Work)
{
	WorkTail->NextLink = Work;
	WorkTail = WorkTail->NextLink;
}

bool CWorkQueue::DeQueue(CWorkBase **Work, unsigned int *Number)
{
	if(IsEmpty() == true) return false;

	if(WorkHead.NextLink->DontIncreaseWorkNumber == false) this->Number++;

	if(Work != NULL) *Work = WorkHead.NextLink;
	if(Number != NULL) *Number = this->Number;

	if(WorkTail == WorkHead.NextLink) WorkTail = &WorkHead;
	WorkHead.NextLink = WorkHead.NextLink->NextLink;

	return true;
}
