#include "stdafx.h"
#include "WorkMessageBase.h"

CWorkMessageBase::CWorkMessageBase(CSimulator *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID) : CWorkBase(Simulator)
{
	this->SrcPeerID = SrcPeerID;
	this->DstPeerID = DstPeerID;

	Message = NULL;
}

CWorkMessageBase::~CWorkMessageBase()
{
	if(Message != NULL) delete Message;
}
