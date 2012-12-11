#pragma once

#include "resource.h"		// �� ��ȣ�Դϴ�.

class CSimApp : public CWinApp
{
public:
	CSimApp();

	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

public:
	CSimulatorEx								Sim;
	unsigned int								RandomSeed;

	ECacheMode									CacheMode;
	EGroupMode									GroupMode;
	unsigned int								MaxFloodHopCount;
	unsigned int								PeerCount;
	unsigned int								SearchContentCount;
	unsigned int								GroupSize;
};

extern CSimApp SimApp;
