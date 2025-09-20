#pragma once
#include "API.h"
#include "ScriptLoader.h"

using std::string;
using std::endl;

//make a loopy script
void Priff_summ() {

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	int pouches_ID = 12155;
	int pouches_amount = 25;
	int spirit_shards_ID = 12183;
	int spirit_shards_amount = 5000;
	int needed_charms_ID = 0;
	int needed_charms_amount = 25;
	int items1 = 0;
	int items2 = 0;
	string Cselect = MEX::ScriptDialogWindow2("Summ", { "Pack yak","Geyser titan" }).Name;
	//Assign stuff here
	if (Cselect.compare("Pack yak") == 0) {
		ScripCuRunning0 = "Pack yaks";
		needed_charms_ID = 12160;
		items1 = 10818;
	}
	if (Cselect.compare("Geyser titan") == 0) {
		ScripCuRunning0 = "Geyser titan";
		needed_charms_ID = 12163;
		items1 = 1444;
	}

	int safecounter1 = 0;
	int safecounter2 = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	while (LoopyLoop) {

		if (InventoryClass.InvStackSize(pouches_ID) > pouches_amount && InventoryClass.InvStackSize(spirit_shards_ID) > spirit_shards_amount && InventoryClass.InvStackSize(needed_charms_ID) > needed_charms_amount && InventoryClass.InvItemFounds({ items1, items2 })) {
			ScripCuRunning1 = "Needed stuff in inventory <------";
			//safety
				safecounter1++;
				console_text << "Infuse try: " << safecounter1 << endl;
				DebugImGui.Debugtext_addstream();
				if (safecounter1 > 11) { DebugImGui.Debugtext_add("Can't infuse stuff"); break; }
				safecounter2 = 0;
			//status:needed stuff in inventory
			//close enough to obelisk
			if (ME::PInArea2(2135, 2143, 3367, 3375)) {
				ScripCuRunning2 = "Close to obby, craft pouches";
				ME::FindObj({ 94230 }, 11, 20, 0, 0, "Infuse");
					ME::RandomSleep2(1200, 2500, 5000);
					ME::KeyboardPress(' ', 40, 100);//with right indrigents it should be highlighted
			}
			else {
				//run to obelisk
				ScripCuRunning2 = "Go to obby";
				ME::Map_Walker1NT({ 2139,3371,0 }, 50);
			}
		}
		else {
			ScripCuRunning1 = "No stuff <------";
			//safety
				safecounter2++;
				console_text << "Bank try: " << safecounter2 << endl;
				DebugImGui.Debugtext_addstream();
				if (safecounter2 > 11) { DebugImGui.Debugtext_add("Can't bank"); break; }
				safecounter1 = 0;
			//status:inventory has not enough needed items
			//close to bank
			if (ME::PInArea2(2150, 2158, 3337, 3345)) {
				ScripCuRunning2 = "Open bank";
				//MEX::OpenBankChest(92692, '1');
			}
			else {
				//run to bank
				ScripCuRunning2 = "Go to bank";
				ME::Map_Walker1NT({ 2153,3341,0 }, 50);
			}
		}

		ME::RandomSleep2(1000, 2050, 3000);
	}
	ScripCuRunning1 = "";
	ScripCuRunning2 = "";
}
