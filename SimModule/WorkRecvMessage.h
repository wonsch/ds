#pragma once

#include "WorkMessageBase.h"

class CWorkRecvMessage : public CWorkMessageBase
{
public:
	CWorkRecvMessage(CSimulator *Simulator, unsigned int SrcPeerID, unsigned int DstPeerID);
	~CWorkRecvMessage();

	virtual	void								Simulate(char *Log, char *Dump);

private:
	CPeerInfo*									DstPeerInfo;

	int											MessageEmpty(char *Log, char *Dump);
	int											MessageNotifyNull(char *Log, char *Dump);
	int											MessageSearchContent(char *Log, char *Dump);
	int											MessageSearchContentResponseSource(char *Log, char *Dump);
	int											MessageSearchContentResponsePath(char *Log, char *Dump);

	/*Jin*/
	int											MessageAskGrouping(char *Log, char *Dump);
	int											MessageRejectGrouping(char *Log, char *Dump);
	int											MessageAcceptGrouping(char *Log, char *Dump);
	int											MessageNotifyGrouping(char *Log, char *Dump);
	int											MessageTryAsking(char *Log, char *Dump);
	/*Jin*/
	
};
