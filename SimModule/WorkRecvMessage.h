#pragma once

#include "WorkMessageBase.h"

class CWorkRecvMessage : public CWorkMessageBase
{
public:
	CWorkRecvMessage(CSimulator *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID);
	~CWorkRecvMessage();

	virtual	void								Simulate(char *Log);

private:
	CPeerInfo*									DstPeerInfo;

	int											MessageEmpty(char *Log);
	int											MessageNotifyNull(char *Log);
	int											MessageSearchContent(char *Log);
	int											MessageSearchContentResponseSource(char *Log);
	int											MessageSearchContentResponsePath(char *Log);

	/*Jin*/
	int											MessageAskGrouping(char* Log);
	int											MessageRejectGrouping(char* Log);
	int											MessageAcceptGrouping(char* Log);
	int											MessageNotifyGrouping(char* Log);
	int											MessageTryAsking(char* Log);
	/*Jin*/
	
};
