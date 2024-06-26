#pragma once
#include "API.h"
#include "ScriptLoader.h"

using std::string;
using std::endl;
using std::to_string;

//make a loopy script
void CitadelWoodChopper() {
	int CitadelLimitVar = 0;
	int CitadelLimitVarOld = 0;
	int CitadelLimitVarCount = 0;
	int CitadelLimitVarCount0 = 0;
	int CitadelLimitId = 1880;

	DebugImGui.Debugtext_add("chops tier 7 tree and tries to watch limit of 2700");
	while (LoopyLoop) {
		ScripCuRunning1 = "Looping";
		MEX::RandomEvents();
		//check limit		
		CitadelLimitVar = ME::VB_FindPSettinOrder(CitadelLimitId).state;
		ScripCuRunning2 = to_string(CitadelLimitVar);
		if (CitadelLimitVar == 0) {
			CitadelLimitVarCount0++;
			if (CitadelLimitVarCount0 > 5) {
				DebugImGui.Debugtext_add("Stopped. Not unlocked citadel?");
				LoopyLoop = FALSE;
			}
		}
		if (CitadelLimitVar >= 270000) {
			DebugImGui.Debugtext_add("Stopped. Reached limit");
			LoopyLoop = FALSE;
		}

		//check for infinite loops
		if (CitadelLimitVarCount >= 10) {
			DebugImGui.Debugtext_add("Stopped. Stuck in a loop?");
			LoopyLoop = FALSE;
		}
		if (CitadelLimitVar == CitadelLimitVarOld) {
			CitadelLimitVarCount++;
		}
		else {
			CitadelLimitVarOld = CitadelLimitVar;
			CitadelLimitVarCount = 0;
		}

		//check movement
		if (!ME::ReadPlayerMovin2()) {
			//check animation
			if (!ME::CheckAnim(100)) {
				//18496,18406
				ScripCuRunning1 = "Chopping";
				ME::FindObj({ 18500,18489 }, 20, 10, FALSE, 0, "Chop");
				ME::RandomSleep2(400, 1250, 2000);
			}
		}
		ME::RandomSleep2(400, 1250, 2000);
	}	
	ScripCuRunning1 = "";
}
