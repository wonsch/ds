#include "stdafx.h"
#include "Simulator.h"

int main()
{
	CSimulator Sim(0);
	Sim.SetEnvironmentRandomly();
	Sim.InsertWorkInsertPeer(1, 50);
	Sim.InsertWorkSearchContent(3);
	Sim.SimulateToInfinity();

	while(true)
	{
		Sim.SimulateCount(1);

		char c = _getch();
		if(c == 'e') break;
	}

	return 0;
}
