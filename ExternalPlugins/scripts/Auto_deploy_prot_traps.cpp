#pragma once
#include "API.h"
#include "ScriptLoader.h"
using std::string;
using std::cout;
using std::endl;

//make a loopy script
void Auto_deploy_prot_traps() {

	int Protean_item1 = 32337;
	int loopprotect1 = 0;
	int loopprotect2 = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	ScripCuRunning1 = "Find a good spot";
	ScripCuRunning2 = "";
	ME::RandomSleep2(4000, 5050, 6000);
	ScripCuRunning2 = "";
	/////////////////////////////////////////////////////////////////////////////////////////////////////


	while (LoopyLoop) {
		MEX::RandomEvents();

		if (loopprotect1 > 3) { cout << "Found nothing to click." << endl; LoopyLoop = false; break; }
		if (loopprotect2 > 5) { cout << "Found no deploy animation." << endl; LoopyLoop = false; break; }

		ME::RandomSleep2(800, 4050, 12000);

		//random mousemovement
		if (ME::Math_RandomNumber(1000) > 940) {
			cout << "idle" << endl;
			ME::PIdle1();
		}

		loopprotect2++;//no animation
		ScripCuRunning1 = "Waiting";
		if (!ME::CheckAnim(400)) {
			ScripCuRunning1 = "Doing";
			if (ME::OpenInventoryInterface2()) {
				loopprotect1++;
				ScripCuRunning1 = "Click 1 normal";
				if (ME::ClickInv_(Protean_item1, 0, 0)) {
					loopprotect1 = 0;
				}
				ME::RandomSleep2(900, 4050, 12000);
			}
			else {
				ScripCuRunning1 = "Inventory not open";
			}
		}
		else {
			if (ME::ReadPlayerAnim() == 26258) {
				ScripCuRunning1 = "Deploy animation detected!!";
				loopprotect2 = 0;
			}
		}

	}
}
