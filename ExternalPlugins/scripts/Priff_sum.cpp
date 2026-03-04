#pragma once
#include "API.h"
#include "ScriptRegistry.h"

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
		console_text << "Pack yaks" << endl;
		DebugImGui.Debugtext_addstream();
		needed_charms_ID = 12160;
		items1 = 10818;
	}
	if (Cselect.compare("Geyser titan") == 0) {
		console_text << "Geyser titan" << endl;
		DebugImGui.Debugtext_addstream();
		needed_charms_ID = 12163;
		items1 = 1444;
	}

	int safecounter1 = 0;
	int safecounter2 = 0;
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	LoopyLoop = true;
	while (LoopyLoop) {

		if (InventoryClass.InvStackSize(pouches_ID) > pouches_amount && InventoryClass.InvStackSize(spirit_shards_ID) > spirit_shards_amount && InventoryClass.InvStackSize(needed_charms_ID) > needed_charms_amount && InventoryClass.InvItemFounds({ items1, items2 })) {
			console_text << "Needed stuff in inventory <------" << endl;
			DebugImGui.Debugtext_addstream();
			//safety
				safecounter1++;
				console_text << "Infuse try: " << safecounter1 << endl;
				DebugImGui.Debugtext_addstream();
				if (safecounter1 > 11) { DebugImGui.Debugtext_add("Can't infuse stuff"); break; }
				safecounter2 = 0;
			//status:needed stuff in inventory
			//close enough to obelisk
			if (ME::PInArea2(2135, 2143, 3367, 3375)) {
				console_text << "Close to obby, craft pouches" << endl;
				DebugImGui.Debugtext_addstream();
				ME::FindObj({ 94230 }, 11, 20, 0, 0, "Infuse");
					ME::RandomSleep2(1200, 2500, 5000);
					ME::KeyboardPress(' ', 40, 100);//with right indrigents it should be highlighted
			}
			else {
				//run to obelisk
				console_text << "Go to obby: " << endl;
				DebugImGui.Debugtext_addstream();
				ME::Map_Walker1NT({ 2139,3371,0 }, 50);
			}
		}
		else {
			console_text << "No stuff <------" << endl;
			DebugImGui.Debugtext_addstream();
			//safety
				safecounter2++;
				console_text << "Bank try: " << safecounter2 << endl;
				DebugImGui.Debugtext_addstream();
				if (safecounter2 > 11) { DebugImGui.Debugtext_add("Can't bank"); break; }
				safecounter1 = 0;
			//status:inventory has not enough needed items
			//close to bank
			if (ME::PInArea2(2150, 2158, 3337, 3345)) {
				console_text << "Open bank" << endl;
				DebugImGui.Debugtext_addstream();
				//MEX::OpenBankChest(92692, '1');
			}
			else {
				//run to bank
				console_text << "Go to bank" << endl;
				DebugImGui.Debugtext_addstream();
				ME::Map_Walker1NT({ 2153,3341,0 }, 50);
			}
		}

		ME::RandomSleep2(1000, 2050, 3000);
	}
}

static ScriptRegistrar g_register_PriffSumm(
	"Priff_summ",
	&Priff_summ,
	"Summoning pouch crafter in Prifddinas (Pack yak or Geyser titan).");
