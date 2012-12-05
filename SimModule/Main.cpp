#include "stdafx.h"
#include "Simulator.h"

int main()
{
	CSimulator Sim;
	Sim.InsertWorkInsertPeer(1, 3);
	Sim.InsertWorkInsertPeer(2, 5);
	Sim.InsertWorkInsertPeer(4, 2);
	Sim.InsertWorkInsertPeer(8, 4);
	Sim.SimulateToInfinity();

	return 0;
}
