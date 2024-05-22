#pragma once
#include "API.h"
#include "ScriptLoader.h"

using std::string;

//make a loopy script
void Portflaskmaker() {

	int item1 = 23193;
	int item2 = 32845;
	int robust_sand = 32847;
	int crystal_sand = 23194;
	int loopprotect1 = 0;
	int loopprotect2 = 0;
	int loopprotect3 = 0;
	bool notemptyinv = false;
	int script = 0;
	int currentflask = 0;

	string Cselect = MEX::ScriptDialogWindow2("Summ", { "Glass make","Blow stuff" }).Name;
	//Assign stuff here
	if (Cselect.compare("Glass make") == 0) {
		ScripCuRunning0 = "Glass make";
		script = 1;
	}
	if (Cselect.compare("Blow stuff") == 0) {
		ScripCuRunning0 = "Blow stuff";
		script = 2;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	ScripCuRunning1 = "Either crystal or robust glass, set to preset 1, lumby. For sandstone put glass make at v";
	ME::RandomSleep2(2000, 3050, 6000);
	/////////////////////////////////////////////////////////////////////////////////////////////////////


	Abilitybar Superglass = MEX::GetABs_name("Superglass");

	while (LoopyLoop) {
		MEX::RandomEvents();

		ME::RandomSleep2(600, 3050, 12000);

		if (loopprotect1 > 4) { LoopyLoop = false; break; }
		if (loopprotect2 > 4) { LoopyLoop = false; break; }

		//random mousemovement
		if (ME::Math_RandomNumber(1000) > 970) {
			DebugImGui.Debugtext_add("idle");
			ME::PIdle1();
		}

		ScripCuRunning1 = "Searching";

		if (ME::OpenInventoryInterface2()) {

			//sandstone// for loading preset 1
			if (script == 1) {
				//if (!ME::CheckAnim(50)) {
				if (Superglass.id == 0) { DebugImGui.Debugtext_add("Superglass hotkey not found"); break; }

					if (ME::InvItemcount_(crystal_sand) > 0 || ME::InvItemcount_(robust_sand) > 0) {
						loopprotect1++;
						loopprotect2 = 0;
						ScripCuRunning1 = "Glass make + " + Superglass.hotkey;
						ME::KeyboardPress3(Superglass.hotkey, 40, 100);
					}
					else {
						//time to bank
						loopprotect2++;
						loopprotect1 = 0;
						ScripCuRunning1 = "Looking for bank(Lumby)";
						MEX::OpenBankChest(79036, '1');
						MEX::OpenBankChest(125115, '1');
					}
					//	}
			}
			if (script == 2) {
				if (!ME::CheckAnim(120)) {
					notemptyinv = false;
					//robust flask
					if (MEX::CheckInvStuff(item1, 0)) {
						currentflask = item1;
						loopprotect1++;
						loopprotect2 = 0;
						notemptyinv = false;
						if (MEX::DoPortables(106594, "Craft")) {
							ScripCuRunning1 = "Do robust flask, port";
							ME::RandomSleep2(1400, 4050, 12000);
							notemptyinv = true;
						}
						else {
							ME::ClickInv_(item1, 0, 0);
							ScripCuRunning1 = "Do robust flask, click";
							ME::RandomSleep2(1400, 4050, 12000);
							ME::KeyboardPress(' ', 40, 100);
							ME::RandomSleep2(500, 2050, 3000);
							notemptyinv = true;
						}
					}

					//crystal flask
					if (MEX::CheckInvStuff(item2, 0)) {
						currentflask = item2;
						loopprotect1++;
						loopprotect2 = 0;
						if (MEX::DoPortables(106594, "Craft")) {
							ScripCuRunning1 = "Do crystal flask, port";
							ME::RandomSleep2(1400, 4050, 12000);
							notemptyinv = true;
						}
						else {
							ME::ClickInv_(item2, 0, 0);
							ScripCuRunning1 = "Do crystal flask, click";
							ME::RandomSleep2(1400, 4050, 12000);
							ME::KeyboardPress(' ', 40, 100);
							ME::RandomSleep2(500, 2050, 3000);
							notemptyinv = true;
						}
					}

					//time to bank
					if (!notemptyinv) {
						loopprotect2++;
						loopprotect1 = 0;
						ScripCuRunning1 = "Looking for bank(Lumby)";
						if (currentflask == 0) {
							ScripCuRunning1 = "Checking";
							DebugImGui.Debugtext_add("Check preset");
							MEX::OpenBankChest(79036, '1');
							MEX::OpenBankChest(125115, '1');
						}
						else {
							if (MEX::OpenBankChest(79036, '1', { currentflask })) {
								loopprotect3 = 0;
								continue;
							}
							else {
								loopprotect3++;
								DebugImGui.Debugtext_add("Item was not found in bank");
								if (loopprotect3 > 3) { LoopyLoop = false; break; }
							}
							if (MEX::OpenBankChest(125115, '1', { currentflask })) {
								loopprotect3 = 0;
								continue;
							}
							else {
								loopprotect3++;
								DebugImGui.Debugtext_add("Item was not found in bank");
								if (loopprotect3 > 3) { LoopyLoop = false; break; }
							}
						}
					}
				}
			}
		}
		else {
			ScripCuRunning1 = "Inventory not open";
		}
	}
	LoopyLoop = false;
	ScripCuRunning1 = "";
}
