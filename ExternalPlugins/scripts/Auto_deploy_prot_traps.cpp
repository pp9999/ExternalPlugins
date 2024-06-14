#pragma once
#include "API.h"
#include "ScriptLoader.h"

using std::string;

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

		if (loopprotect1 > 3) { DebugImGui.Debugtext_add("Found nothing to click."); LoopyLoop = false; break; }
		if (loopprotect2 > 5) { DebugImGui.Debugtext_add("Found no deploy animation."); LoopyLoop = false; break; }

		ME::RandomSleep2(800, 4050, 12000);

		//random mousemovement
		if (ME::Math_RandomNumber(1000) > 940) {
			DebugImGui.Debugtext_add("idle");
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
