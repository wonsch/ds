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
<<<<<<< HEAD
	int											MessageAskGrouping(char* Log);
	int											MessageRejectGrouping(char* Log);
	int											MessageAcceptGrouping(char* Log);
	int											MessageNotifyGrouping(char* Log);
	int											MessageTryAsking(char* Log);
	int											MessageUpdateContentInfo(char* Log);
=======
	int											MessageAskGrouping(char *Log, char *Dump);
	int											MessageRejectGrouping(char *Log, char *Dump);
	int											MessageAcceptGrouping(char *Log, char *Dump);
	int											MessageNotifyGrouping(char *Log, char *Dump);
	int											MessageTryAsking(char *Log, char *Dump);
>>>>>>> a1e5e6a68deda85a01bf0180689d72717837e5ee
	/*Jin*/
	
};
