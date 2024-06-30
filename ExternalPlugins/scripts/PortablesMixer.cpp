#pragma once
#include "API.h"
#include "ScriptLoader.h"

using namespace std;

//click stuff
static bool MixHerbsManually(int item1) {
	ScripCuRunning2 = "MixHerbsByHand";
	if (MEX::CheckDoItemOpen()) {
		ME::KeyboardPress(' ', 40, 100);
		return true;
	}
	if (ME::ClickInv_(item1, 0,0)) {
		ME::RandomSleep2(900, 3500, 5000);
		ME::KeyboardPress(' ', 40, 100);
		return true;
	}
	return false;
}


//make a loopy script
void HerblawTest() {

	BOOLEAN mixbyhand = false;
	BOOLEAN mixbyhandonly = false;
	int item1 = 0;
	int item2 = 0;
	int item3 = 0;
	int item4 = 0;
	int item5 = 0;
	int item6 = 0;
	int item7 = 0;
	int type = 0;
	int loopprotect1 = 0;
	int loopprotect2 = 0;
	bool scriptdebug = true;
	int starting_amount = 0;
	bool start_count_done = false;
	int end_limit = 0;//how much items to use
	int except_size = 3;
	int except_size2 = 0;
	bool oneslotitem = false;

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	string Cselect = MEX::ScriptDialogWindow2("Do Herb type", {
"Super prayer renewal",
"Elder overload salve",
"Elder Overload",
"Supreme overload salve",
"Supreme overload",
"Holy overload potion",
"Aggroverload",
"Standard Overload",
"Extreme ranging potion",
"Extreme magic potion",
"Extreme defence potion",
"Extreme strength potion",
"Extreme attack potion",
"fellstalk potion (unf)",
"Prayer renewal",
"Super antipoison potion",
"Antifire potion",
"Super Antifire potion",
"dwarf weed potion (unf)",
"Super ranging potion",
"lantadyme potion (unf)",
"Super magic potion",
"cadantine potion (unf)",
"Super defence potion",
"kwuarm potion (unf)",
"Super strength potion",
"irit potion (unf)",
"Super attack potion",
"toadflax potion (unf)",
"Saradomin brew",
"ranarr potion (unf)",
"Prayer potion",
"snapdragon potion (unf)",
"Super restore potion",
"bloodweed potion (unf)",
"Aggression potion",
"wergali potion (unf)",
"Runecrafting potion summer",
"Runecrafting potion fall",
"Super Runecrafting potion",
"Powerburst of sorcery",
"Clean lantadyme",
"Clean irit",
"Clean kwuarm",
"Clean cadantine",
"Clean toadflax",
"Clean ranarr",
"Clean snapdragon",
"Clean dwarf weed",
"Clean fellstalk",
"Clean torstol",
"Clean arbuck",
"Clean bloodweed",
"primal pulp into vials",
"primal pulp from fruits",
"Protean shakes",
"grind birb nests",
"Regular ink",
"Greater ink",
"Powerful ink"
}).Name;

	//Assign stuff here
	if (Cselect.compare("Powerburst of sorcery") == 0) {
		ScripCuRunning1 = "Powerburst of sorcery";
		item1 = 48960;
		item2 = 48925;
		item3 = 48962;
		item4 = 565;
		item5 = 44079;
		except_size = 0;
		except_size2 = 1;
		mixbyhand = false;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Super Runecrafting potion") == 0) {
		ScripCuRunning1 = "Super Runecrafting potion";
		item1 = 44055;
		item2 = 43989;
		type = 1;
		mixbyhand = false;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Runecrafting potion summer") == 0) {
		ScripCuRunning1 = "Runecrafting potion summer";
		item1 = 14856;
		item2 = 43975;
		type = 1;
		mixbyhand = false;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Runecrafting potion fall") == 0) {
		ScripCuRunning1 = "Runecrafting potion fall";
		item1 = 14856;
		item2 = 43981;
		type = 1;
		mixbyhand = false;
		mixbyhandonly = false;
	}
	if (Cselect.compare("wergali potion (unf)") == 0) {
		ScripCuRunning1 = "wergali potion (unf)";
		item1 = 227;
		item2 = 14854;
		type = 1;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Powerful ink") == 0) {
		ScripCuRunning1 = "Powerful ink";
		item1 = 55601;
		item2 = 592;
		item3 = 227;
		type = 1;
		except_size = 0;
		mixbyhand = true;
		mixbyhandonly = true;
		oneslotitem = false;
	}
	if (Cselect.compare("Greater ink") == 0) {
		ScripCuRunning1 = "Greater ink";
		item1 = 55600;
		item2 = 592;
		item3 = 227;
		type = 1;
		except_size = 0;
		mixbyhand = true;
		mixbyhandonly = true;
		oneslotitem = false;
	}
	if (Cselect.compare("Regular ink") == 0) {
		ScripCuRunning1 = "Regular ink";
		item1 = 55599;
		item2 = 592;
		item3 = 227;
		type = 1;
		except_size = 0;
		mixbyhand = true;
		mixbyhandonly = true;
		oneslotitem = false;
	}
	if (Cselect.compare("grind birb nests") == 0) {
		ScripCuRunning1 = "grind birb nests";
		item1 = 5075;
		item2 = 0;
		type = 1;
		except_size = 0;
		mixbyhand = true;
		mixbyhandonly = true;
		oneslotitem = false;
	}
	if (Cselect.compare("Protean shakes") == 0) {
		ScripCuRunning1 = "Protean shakes";
		item1 = 53129;
		item2 = 0;
		type = 1;
		except_size = 0;
		mixbyhand = true;
		mixbyhandonly = true;
		oneslotitem = true;

	}
	/*
		if (Cselect.compare("Clean ") == 0) {
		ScripCuRunning1 = "Clean ";
		item1 = ;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare(" potion (unf)") == 0) {
		ScripCuRunning1 = " potion (unf)";
		item1 = 227;
		item2 = ;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare(" potion") == 0) {
		ScripCuRunning1 = " potion";
		item1 = ;
		item2 = ;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	*/
	if (Cselect.compare("Super prayer renewal") == 0) {
		ScripCuRunning1 = "Super prayer renewal";
		item1 = 32843;
		item2 = 21630;
		item3 = 2434;
		type = 1;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Elder overload salve") == 0) {
		ScripCuRunning1 = "Elder overload salve";
		item1 = 49039;
		item2 = 21630;
		item3 = 2434;
		item4 = 2448;
		item5 = 2452;
		item6 = 15304;
		type = 1;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Elder Overload") == 0) {
		ScripCuRunning1 = "Elder Overload";
		item1 = 48962;
		item2 = 33210;
		item3 = 21624;
		type = 1;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Supreme overload salve") == 0) {
		ScripCuRunning1 = "Supreme overload salve";
		item1 = 32843;
		item2 = 33210;
		item3 = 21630;
		item4 = 2434;
		item5 = 2448;
		item6 = 2452;
		item7 = 15304;
		type = 1;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Supreme overload") == 0) {
		ScripCuRunning1 = "Supreme overload";
		item1 = 32843;
		item2 = 15332;
		item3 = 2436;
		item4 = 2442;
		item5 = 2440;
		item6 = 2444;
		item7 = 3040;
		type = 1;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Holy overload potion") == 0) {
		ScripCuRunning1 = "Holy overload potion";
		item1 = 32843;
		item2 = 21630;
		item3 = 15332;
		type = 1;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Aggroverload") == 0) {
		ScripCuRunning1 = "Aggroverload";
		item1 = 32843;
		item2 = 37971;
		item3 = 15332;
		item3 = 48211;
		type = 1;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	//game will auto pick 120 cape option
	if (Cselect.compare("Standard Overload") == 0) {
		ScripCuRunning1 = "Standard Overload";
		item1 = 269;
		item2 = 15317;
		item3 = 15321;
		item4 = 15325;
		item5 = 15313;
		item6 = 15309;
		type = 1;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Extreme ranging potion") == 0) {
		ScripCuRunning1 = "Extreme ranging potion";
		item1 = 12539;
		item2 = 169;
		mixbyhand = true;
		mixbyhandonly = false;
		except_size = 0;
	}
	if (Cselect.compare("Extreme magic potion") == 0) {
		ScripCuRunning1 = "Extreme magic potion";
		item1 = 9594;
		item2 = 3042;
		mixbyhand = true;
		mixbyhandonly = false;
		except_size = 0;
	}
	if (Cselect.compare("Extreme defence potion") == 0) {
		ScripCuRunning1 = "Extreme defence potion";
		item1 = 2481;
		item2 = 163;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Extreme strength potion") == 0) {
		ScripCuRunning1 = "Extreme strength potion";
		item1 = 267;
		item2 = 157;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Extreme attack potion") == 0) {
		ScripCuRunning1 = "Extreme attack potion";
		item1 = 261;
		item2 = 145;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("fellstalk potion (unf)") == 0) {
		ScripCuRunning1 = "fellstalk potion (unf)";
		item1 = 227;
		item2 = 21624;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Prayer renewal") == 0) {
		ScripCuRunning1 = "Prayer renewal";
		item1 = 21628;
		item2 = 21622;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Super antipoison potion") == 0) {
		ScripCuRunning1 = "Super antipoison potion";
		item1 = 101;
		item2 = 235;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Antifire potion") == 0) {
		ScripCuRunning1 = "Antifire potion";
		item1 = 2483;
		item2 = 241;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Super Antifire potion") == 0) {
		ScripCuRunning1 = "Super Antifire potion";
		item1 = 4621;
		item2 = 2454;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("dwarf weed potion (unf)") == 0) {
		ScripCuRunning1 = "dwarf weed potion (unf)";
		item1 = 227;
		item2 = 267;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Super ranging potion") == 0) {
		ScripCuRunning1 = "Super ranging potion";
		item1 = 109;
		item2 = 245;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("lantadyme potion (unf)") == 0) {
		ScripCuRunning1 = "lantadyme potion (unf)";
		item1 = 227;
		item2 = 2481;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Super magic potion") == 0) {
		ScripCuRunning1 = "Super magic potion";
		item1 = 2483;
		item2 = 3138;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("cadantine potion (unf)") == 0) {
		ScripCuRunning1 = "cadantine potion (unf)";
		item1 = 227;
		item2 = 265;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Super defence potion") == 0) {
		ScripCuRunning1 = "Super defence potion";
		item1 = 107;
		item2 = 239;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("kwuarm potion (unf)") == 0) {
		ScripCuRunning1 = "kwuarm potion (unf)";
		item1 = 227;
		item2 = 263;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Super strength potion") == 0) {
		ScripCuRunning1 = "Super strength potion";
		item1 = 105;
		item2 = 225;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("irit potion (unf)") == 0) {
		ScripCuRunning1 = "irit potion (unf)";
		item1 = 227;
		item2 = 259;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Super attack potion") == 0) {
		ScripCuRunning1 = "Super attack potion";
		item1 = 101;
		item2 = 221;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("toadflax potion (unf)") == 0) {
		ScripCuRunning1 = "toadflax potion (unf)";
		item1 = 227;
		item2 = 2998;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Saradomin brew") == 0) {
		ScripCuRunning1 = "Saradomin brew";
		item1 = 3002;
		item2 = 6693;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("ranarr potion (unf)") == 0) {
		ScripCuRunning1 = "ranarr potion (unf)";
		item1 = 227;
		item2 = 257;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Prayer potion") == 0) {
		ScripCuRunning1 = "Prayer potion";
		item1 = 99;
		item2 = 231;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("snapdragon potion (unf)") == 0) {
		ScripCuRunning1 = "snapdragon potion (unf)";
		item1 = 227;
		item2 = 3000;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Super restore potion") == 0) {
		ScripCuRunning1 = "Super restore potion";
		item1 = 3004;
		item2 = 223;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("bloodweed potion (unf)") == 0) {
		ScripCuRunning1 = "bloodweed potion (unf)";
		item1 = 48962;
		item2 = 37953;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	if (Cselect.compare("Aggression potion") == 0) {
		ScripCuRunning1 = "Aggression potion";
		item1 = 37973;
		item2 = 34159;
		mixbyhand = true;
		mixbyhandonly = false;
	}
	/////////////////////////////////////////////
	if (Cselect.compare("Clean lantadyme") == 0) {
		ScripCuRunning1 = "Clean lantadyme";
		item1 = 2485;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean irit") == 0) {
		ScripCuRunning1 = "Clean irit";
		item1 = 209;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean kwuarm") == 0) {
		ScripCuRunning1 = "Clean kwuarm";
		item1 = 213;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean cadantine") == 0) {
		ScripCuRunning1 = "Clean cadantine";
		item1 = 215;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean toadflax") == 0) {
		ScripCuRunning1 = "Clean toadflax";
		item1 = 3049;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean ranarr") == 0) {
		ScripCuRunning1 = "Clean ranarr";
		item1 = 207;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean snapdragon") == 0) {
		ScripCuRunning1 = "Clean snapdragon";
		item1 = 3051;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean dwarf weed") == 0) {
		ScripCuRunning1 = "Clean dwarf weed";
		item1 = 217;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean fellstalk") == 0) {
		ScripCuRunning1 = "Clean fellstalk";
		item1 = 21626;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean torstol") == 0) {
		ScripCuRunning1 = "Clean torstol";
		item1 = 219;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean arbuck") == 0) {
		ScripCuRunning1 = "Clean arbuck";
		item1 = 48243;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("Clean bloodweed") == 0) {
		ScripCuRunning1 = "Clean bloodweed";
		item1 = 37975;
		item2 = 0;
		mixbyhand = true;
		mixbyhandonly = true;
	}
	if (Cselect.compare("primal pulp into vials") == 0) {
		ScripCuRunning1 = "primal pulp into vials";
		item1 = 48966;
		item2 = 229;
		//48962
		mixbyhand = true;
		mixbyhandonly = false;
		except_size = 0;
		except_size2 = 1;
	}
	if (Cselect.compare("primal pulp from fruits") == 0) {
		ScripCuRunning1 = "primal pulp from fruits";
		item1 = 0;
		mixbyhand = true;
		mixbyhandonly = false;
		vector<int>ids = { 48578,48582,48584,48586,48773 };
		vector<bool>salad = MEX::CheckInvStuff(ids, 0);
		for (int i = 0; i < salad.size(); i++) {
			if (salad[i]) {
				item1 = ids[i];
				break;
			}
		}
		if (item1 == 0) { DebugImGui.Debugtext_add("No fruits"); LoopyLoop = false; }
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//fake_mouse_do = true;
	while (LoopyLoop) {
		if (RS_Injected) {
			DO::RandomEvents();
		}
		else {
			MEX::RandomEvents();
		}

		ME::RandomSleep2(1500, 4050, 12000);

		if (loopprotect2 > 5) { if (scriptdebug) { DebugImGui.Debugtext_add("can't open bank"); }LoopyLoop = false; continue; }
		if (loopprotect1 > 5) { if (scriptdebug) { DebugImGui.Debugtext_add("can't mix"); }LoopyLoop = false; continue; }


		if (!ME::CheckAnim(120)) {
			ScripCuRunning2 = "Searching";
			if (ME::OpenInventoryInterface2()) {
				if (MEX::CheckInvStuffCheckAll({ item1,item2,item3,item4,item5,item6,item7 }, except_size)) {
					loopprotect1++;
					if (oneslotitem) { loopprotect1 = 0; }
					loopprotect2 = 0;
					if (mixbyhandonly) {
						if (scriptdebug) { DebugImGui.Debugtext_add("mix herbs manually"); }
						MixHerbsManually(item1);
					}
					else
					{
						if (MEX::DoPortables(89770, "Mix")) {
							ScripCuRunning2 = "Mix herbs";
							if (scriptdebug) { DebugImGui.Debugtext_add("mix herbs port"); }
						}
						else {
							if (mixbyhand) {
								if (scriptdebug) { DebugImGui.Debugtext_add("mix herbs, port not found"); }
								MixHerbsManually(item1);
							}
						}
					}
				}
				else {
					loopprotect2++;
					if (oneslotitem) { DebugImGui.Debugtext_add("Out of item?"); LoopyLoop = false; continue;}
					loopprotect1 = 0;
					if (scriptdebug) { DebugImGui.Debugtext_add("No stuff, trying to open bank"); }
					vector <uint64_t> count_array = MEX::OpenBankChest_am(79036, '1',{ item1,item2,item3,item4,item5,item6,item7 }, except_size);
					if (!count_array.empty()) {
						if (!start_count_done) {
							start_count_done = true;
							starting_amount = count_array.front();
						}
						if (end_limit > 0) {
							if (count_array.front() < starting_amount - end_limit) {
								LoopyLoop = false;
								DebugImGui.Debugtext_add("Reached limit");
								break;
							}
						}
					}
					vector <uint64_t> count_array2 = MEX::OpenBankChest_am(125115, '1', { item1,item2,item3,item4,item5,item6,item7 }, except_size);
					if (!count_array2.empty()) {
						if (!start_count_done) {
							start_count_done = true;
							starting_amount = count_array2.front();
						}
						if (end_limit > 0) {
							if (count_array2.front() < starting_amount - end_limit) {
								LoopyLoop = false;
								DebugImGui.Debugtext_add("Reached limit");
								break;
							}
						}
					}
				}
			}
			else {
				ScripCuRunning1 = "Inventory not open";
				if (scriptdebug) { DebugImGui.Debugtext_add("Inventory not open"); }
			}
		}
	}
}
