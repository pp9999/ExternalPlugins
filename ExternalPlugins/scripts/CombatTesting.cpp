#pragma once
#include "API.h"
#include "ScriptLoader.h"
#include <thread>

using namespace std;

static int failure_rate = 0;
static int combat_check = 0;
static string setting_list = "setting.txt";
static string setting_folder = "Generic";
static string NPC_folder = "NPC_list";
static int area = 0;
static FFPOINT starttile{};
static bool show_area = false;
static ImColor col = ImColor(0, 128, 128);
static ImColor col2 = ImColor(255, 255, 255);
static ImColor backdrop = ImColor(0, 0, 0, 120);
static bool Use_quickprays = false;
static bool Silent_Use_quickprays = false;
static bool Eat_foods = false;
static bool Silent_Eat_foods = false;
static int Check_space = 0;
static bool IgnoreInCombat = false;
static int Health_precentage = 0;
static int Pray_precentage = 0;
static vector<int> Rare_D{};
static bool Pickup_D = false;
static bool bool_note = false;
static bool bool_alch = false;
static bool bool_drop = false;
static bool bool_excalibur = false;
static bool bool_attack = false;
static bool bool_pickup = false;
static bool bool_eatfoodstomakespace = false;
static bool bool_endfull = false;
static bool bool_focusonnpc = false;


void Push_rare_drops() {
	Rare_D.clear();
	Rare_D.push_back(18778);//Starved ancient effigy
	Rare_D.push_back(42006);//Sealed clue scroll easy
	Rare_D.push_back(42007);//Sealed clue scroll medium
	Rare_D.push_back(42008);//Sealed clue scroll hard
	Rare_D.push_back(42009);//Sealed clue scroll elite
	Rare_D.push_back(42010);//Sealed clue scroll master
	//Rare_D.push_back(1617);//Uncut diamond
	//Rare_D.push_back(1631);//Uncut dragonstone
	Rare_D.push_back(985);//Loop half of a key
	Rare_D.push_back(987);//Tooth half of a key
	Rare_D.push_back(378);//raw lobster
	Rare_D.push_back(2366);//Shield left half
	Rare_D.push_back(5289);//Palm tree seed
	Rare_D.push_back(5316);//Magic seed
	Rare_D.push_back(5300);//Snapdragon seed
	Rare_D.push_back(5302);//Lantadyme seed
	Rare_D.push_back(5303);//Dwarf weed seed
	Rare_D.push_back(5304);//Torstol seed
	Rare_D.push_back(47314);//Huge rune salvage
	Rare_D.push_back(1516);//Yew logs
	//Rare_D.push_back(44);//Rune arrowheads
	Rare_D.push_back(1149);//Dragon helm
	Rare_D.push_back(1305);//Dragon longsword
	Rare_D.push_back(25740);//Dragon off-longsword
	Rare_D.push_back(1249);//Dragon spear
	//Rare_D.push_back(44808);//Runite stone spirits
	//Rare_D.push_back(44809);//Orichaclh stone spirits
	//Rare_D.push_back(44810);//Drakolith stone spirits
	//Rare_D.push_back(44811);//Necrite stone spirits
	//Rare_D.push_back(44812);//Phasmatite stone spirits
	//Rare_D.push_back(44813);//Banite stone spirits
	//Rare_D.push_back(44814);//Light anime stone spirits
	//Rare_D.push_back(44815);//Dark anime stone spirits
	Rare_D.push_back(1776);//Molten glass
	Rare_D.push_back(1780);//Flax
	Rare_D.push_back(1762);//Soft clay
	Rare_D.push_back(384);//Raw shark
	Rare_D.push_back(47315);//Huge rune salvage
	Rare_D.push_back(8781);//Teak plank
	Rare_D.push_back(8783);//Mahogany plank
	Rare_D.push_back(533);//Big bones
	Rare_D.push_back(537);//Dragon bones
	Rare_D.push_back(9342);//Onyx bolts
	Rare_D.push_back(2435);//Pray potion(4)
	Rare_D.push_back(3025);//Super restore(4)
	Rare_D.push_back(48576);//Inert adreline crystal
	Rare_D.push_back(1441);//Earth talisman
	Rare_D.push_back(1445);//Water talisman
	Rare_D.push_back(1392);//Battlestaff
	Rare_D.push_back(1618);//Uncut diamond
	Rare_D.push_back(1632);//Uncut dragonstone
	Rare_D.push_back(20667);//Vecna skull
	Rare_D.push_back(2064);//Blurberry special
	Rare_D.push_back(2259);//Cheese+tom batta
	Rare_D.push_back(39814);//Hazelmere's signet ring
}

//loop it
//update health and pray values here asewell
static bool bool_clear_tiles = false;//later then
static void ScriptPaint() {

	bool loadup = false;
	int ask_int = 0;
	bool ask_bool = false;	
	int refresh_count = 0;
	starttile = ME::PlayerCoordfloat();

	IG_answer IG_back{};
	IG_back.box_name = "back";
	IG_back.box_start = { 0,50,0 };
	IG_back.box_size = { 160,375,0 };
	IG_back.colour = backdrop;
	IG_answer PRAY_textt{};
	PRAY_textt.box_name = "PRAY_textt";
	PRAY_textt.box_start = { 25,55,0 };
	PRAY_textt.colour = col2;
	IG_answer HEALTH_textt{};
	HEALTH_textt.box_name = "HEALTH_textt";
	HEALTH_textt.box_start = { 25,70,0 };
	HEALTH_textt.colour = col2;
	IG_answer AttackNPCmanager{};
	AttackNPCmanager.box_name = "Attack NPC manager";
	AttackNPCmanager.box_start = { 0,100,0 };
	AttackNPCmanager.box_size = { 160,30,0 };
	IG_answer NPCattack{};
	NPCattack.box_name = "NPCattack";
	NPCattack.box_start = { 152,100,0 };
	NPCattack.box_size = { 20,20,0 };
	IG_answer PickupItemsmanager{};
	PickupItemsmanager.box_name = "Pickup Items manager";
	PickupItemsmanager.box_start = { 0,120,0 };
	PickupItemsmanager.box_size = { 160,30,0 };
	IG_answer Itemspickup{};
	Itemspickup.box_name = "Itemspickup";
	Itemspickup.box_start = { 152,120,0 };
	Itemspickup.box_size = { 20,20,0 };
	IG_answer Notemanager{};
	Notemanager.box_name = "Note manager";
	Notemanager.box_start = { 0,140,0 };
	Notemanager.box_size = { 160,30,0 };
	IG_answer NoteStuff{};
	NoteStuff.box_name = "NoteStuff";
	NoteStuff.box_start = { 152,140,0 };
	NoteStuff.box_size = { 20,20,0 };
	IG_answer Alchmanager{};
	Alchmanager.box_name = "Alch manager";
	Alchmanager.box_start = { 0,160,0 };
	Alchmanager.box_size = { 160,30,0 };
	IG_answer AlchStuff{};
	AlchStuff.box_name = "AlchStuff";
	AlchStuff.box_start = { 152,160,0 };
	AlchStuff.box_size = { 20,20,0 };
	IG_answer Dropmanager{};
	Dropmanager.box_name = "Drop manager";
	Dropmanager.box_start = { 0,180,0 };
	Dropmanager.box_size = { 160,30,0 };
	IG_answer DropStuff{};
	DropStuff.box_name = "DropStuff";
	DropStuff.box_start = { 152,180,0 };
	DropStuff.box_size = { 20,20,0 };
	IG_answer Usequickprays{};
	Usequickprays.box_name = "Use quickprays";
	Usequickprays.box_start = { 0,200,0 };
	Usequickprays.box_size = { 160,20,0 };
	IG_answer Eatfoods{};
	Eatfoods.box_name = "Eat foods";
	Eatfoods.box_start = { 0,220,0 };
	Eatfoods.box_size = { 160,20,0 };
	IG_answer IGIgnoreInCombat{};
	IGIgnoreInCombat.box_name = "IgnoreInCombat";
	IGIgnoreInCombat.box_start = { 0,240,0 };
	IGIgnoreInCombat.box_size = { 160,20,0 };
	IG_answer RareDrops{};
	RareDrops.box_name = "RareDrops";
	RareDrops.box_start = { 0,260,0 };
	RareDrops.box_size = { 160,20,0 };
	IG_answer UseExalibur{};
	UseExalibur.box_name = "UseExalibur";
	UseExalibur.box_start = { 0, 280, 0 };
	UseExalibur.box_size = { 160,20,0 };
	IG_answer Lookingrange{};
	Lookingrange.box_name = "Looking range";
	Lookingrange.box_start = { 0,80,0 };
	Lookingrange.box_size = { 160,30,0 };
	IG_answer IGArea{};
	IGArea.box_name = "Area";
	IGArea.box_start = { 160,90,0 };
	IGArea.box_size = { 160,80,0 };
	IG_answer IGAreaDraw1{};
	IGAreaDraw1.radius = 10;
	IGAreaDraw1.colour = col;
	IGAreaDraw1.thickness = 1;
	IG_answer IGAreaDraw2{};
	IGAreaDraw2.radius = 10;
	IGAreaDraw2.colour = col;
	IGAreaDraw2.thickness = 1;
	IG_answer IGAreaDraw3{};
	IGAreaDraw3.radius = 10;
	IGAreaDraw3.colour = col;
	IGAreaDraw3.thickness = 1;
	IG_answer IGAreaDraw4{};
	IGAreaDraw4.radius = 10;
	IGAreaDraw4.colour = col;
	IGAreaDraw4.thickness = 1;
	IG_answer eatfoodspace{};
	eatfoodspace.box_name = "eatfoodspace";//eat food to get inventory space
	eatfoodspace.box_start = { 0,300,0 };
	eatfoodspace.box_size = { 160,20,0 };
	IG_answer outofspaceend{};
	outofspaceend.box_name = "outofspaceend";//end if out of space
	outofspaceend.box_start = { 0,320,0 };
	outofspaceend.box_size = { 160,20,0 };
	IG_answer focusfirstnpc{};
	focusfirstnpc.box_name = "focusfirstnpc";//focus on first npc on list
	focusfirstnpc.box_start = { 0,340,0 };
	focusfirstnpc.box_size = { 160,20,0 };


	while (LoopyLoop) {

		if (!loadup) {
			Push_rare_drops();
			loadup = true;
			IG::load_AskNPCs(NPC_folder);
			console_text << dec << "NPCs loaded:" << ask_npcready.size() << endl;
			DebugImGui.Debugtext_addstream();
			if (ask_npcready.size() > 0) {
				setting_folder = ask_npcready[0];
			}
			area = IG::LoadIntSetting(setting_folder, setting_list, 0);
			ask_int = area;
			IGArea.int_value = area;
			console_text << dec << "Range:" << area << endl;
			DebugImGui.Debugtext_addstream();
			IG::load_AskGr(setting_folder);
			console_text << dec << "Grounditems loaded:" << ask_list_Gr_ids_text.size() << endl;
			DebugImGui.Debugtext_addstream();
			IG::load_AskGr_note(setting_folder);
			IG::load_AskGr_alch(setting_folder);
			IG::load_AskGr_drop(setting_folder);
			Usequickprays.box_ticked = Use_quickprays = IG::LoadIntSetting(setting_folder, setting_list, 1);
			Eatfoods.box_ticked = Eat_foods = IG::LoadIntSetting(setting_folder, setting_list, 2);
			IGIgnoreInCombat.box_ticked = IgnoreInCombat = IG::LoadIntSetting(setting_folder, setting_list, 3);
			RareDrops.box_ticked = Pickup_D = IG::LoadIntSetting(setting_folder, setting_list, 4);//rare drops
			NoteStuff.box_ticked = bool_note = IG::LoadIntSetting(setting_folder, setting_list, 5);
			AlchStuff.box_ticked = bool_alch = IG::LoadIntSetting(setting_folder, setting_list, 6);
			DropStuff.box_ticked = bool_drop = IG::LoadIntSetting(setting_folder, setting_list, 7);
			UseExalibur.box_ticked = bool_excalibur = IG::LoadIntSetting(setting_folder, setting_list, 8);
			NPCattack.box_ticked = bool_attack = IG::LoadIntSetting(setting_folder, setting_list, 9);
			Itemspickup.box_ticked = bool_pickup = IG::LoadIntSetting(setting_folder, setting_list, 10);//items manager
			eatfoodspace.box_ticked = bool_eatfoodstomakespace = IG::LoadIntSetting(setting_folder, setting_list, 11);
			outofspaceend.box_ticked = bool_endfull = IG::LoadIntSetting(setting_folder, setting_list, 12);
			focusfirstnpc.box_ticked = bool_focusonnpc = IG::LoadIntSetting(setting_folder, setting_list, 13);
		}

		if (ask_npcready.size() > 0) {
			setting_folder = ask_npcready[0];
		}
		
		IG::DrawSquareFilled(true, &IG_back);

		refresh_count++;
		if (refresh_count > 10) {
			refresh_count = 0;
			//get health % constantly, it should vary between 0-120 
			Health_precentage = ME::GetHPrecent();
			Pray_precentage = ME::GetPrayPrecent();
		}

		HEALTH_textt.string_value = "HEALTH%: " + to_string(Health_precentage);
		IG::DrawTextAt(false, &HEALTH_textt);
		PRAY_textt.string_value = "PRAY%: " + to_string(Pray_precentage);
		IG::DrawTextAt(false, &PRAY_textt);

		//ask
		IG::DrawBox(true, &AttackNPCmanager);
		if (AttackNPCmanager.return_click) {
			AttackNPCmanager.return_click = false;//turn manually off
			IG::DrawAskNPCs(true, NPC_folder);//just use global arrays
		}
		IG::DrawCheckbox(true, &NPCattack);
		if (NPCattack.return_click){
			NPCattack.return_click = false;//turn manually off
			bool_attack = NPCattack.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 9, bool_attack);
		}

		//ask
		IG::DrawBox(true, &PickupItemsmanager);
		if (PickupItemsmanager.return_click) {
			PickupItemsmanager.return_click = false;//turn manually off
			IG::DrawAskGr(true, setting_folder);//just use global arrays
		}

		IG::DrawCheckbox(true, &Itemspickup);
		if (Itemspickup.return_click) {
			Itemspickup.return_click = false;//turn manually off
			bool_pickup = Itemspickup.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 10, bool_pickup);
		}

		//ask
		IG::DrawBox(true, &Notemanager);
		if (Notemanager.return_click) {
			Notemanager.return_click = false;//turn manually off
			IG::DrawAskGr_note(true, setting_folder);//just use global arrays
		}
		//notepaper in inventory
		//box5 = bool_note;
		IG::DrawCheckbox(true, &NoteStuff);
		if (NoteStuff.return_click) {
			NoteStuff.return_click = false;//turn manually off
			bool_note = NoteStuff.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 5, bool_note);
		}

		//ask
		IG::DrawBox(true, &Alchmanager);
		if (Alchmanager.return_click) {
			Alchmanager.return_click = false;//turn manually off
			IG::DrawAskGr_alch(true, setting_folder);//just use global arrays
		}
		//alch on ability bar
		//box6 = bool_note;
		IG::DrawCheckbox(true, &AlchStuff);
		if (AlchStuff.return_click) {
			AlchStuff.return_click = false;//turn manually off
			bool_alch = AlchStuff.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 6, bool_alch);
		}

		//ask
		IG::DrawBox(true, &Dropmanager);
		if (Dropmanager.return_click) {
			Dropmanager.return_click = false;//turn manually off
			IG::DrawAskGr_drop(true, setting_folder);//just use global arrays
		}
		//
		IG::DrawCheckbox(true, &DropStuff);
		if (DropStuff.return_click) {
			DropStuff.return_click = false;//turn manually off
			bool_drop = DropStuff.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 7, bool_drop);
		}

		//do prayer managment
		IG::DrawCheckbox(true, &Usequickprays);
		if (Usequickprays.return_click) {
			Usequickprays.return_click = false;//turn manually off
			Use_quickprays = Usequickprays.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 1, Use_quickprays);
		}

		//do healing
		IG::DrawCheckbox(true, &Eatfoods);
		if (Eatfoods.return_click) {
			Eatfoods.return_click = false;//turn manually off
			Eat_foods = Eatfoods.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 2, Eat_foods);
		}

		//ignore combat stance and go with health varbit
		IG::DrawCheckbox(true, &IGIgnoreInCombat);
		if (IGIgnoreInCombat.return_click) {
			IGIgnoreInCombat.return_click = false;//turn manually off
			IgnoreInCombat = IGIgnoreInCombat.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 3, IgnoreInCombat);
		}

		//pickup rare drop table
		IG::DrawCheckbox(true, &RareDrops);
		if (RareDrops.return_click) {
			RareDrops.return_click = false;//turn manually off
			Pickup_D = RareDrops.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 4, Pickup_D);
		}

		//
		//box8 = bool_excalibur;
		IG::DrawCheckbox(true, &UseExalibur);
		if (UseExalibur.return_click) {
			UseExalibur.return_click = false;//turn manually off
			bool_excalibur = UseExalibur.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 8, bool_excalibur);
		}

		//ask
		IG::DrawBox(true, &Lookingrange);
		if (Lookingrange.return_click) {
			Lookingrange.return_click = false;//turn manually off
			IG::DrawAskInt(true, &IGArea);
			if (show_area) {
				show_area = false;
			}
			else {
				show_area = true;
			}
		}
		if (IGArea.return_click) {
			IGArea.return_click = false;
			area = IGArea.int_value;
			ask_int = IGArea.int_value;
			starttile = ME::PlayerCoordfloat();
			IG::SaveIntSetting(setting_folder, setting_list, 0, area);
		}

		//
		IG::DrawCheckbox(true, &eatfoodspace);
		if (eatfoodspace.return_click) {
			eatfoodspace.return_click = false;//turn manually off
			bool_eatfoodstomakespace = eatfoodspace.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 11, bool_eatfoodstomakespace);
		}
		//
		IG::DrawCheckbox(true, &outofspaceend);
		if (outofspaceend.return_click) {
			outofspaceend.return_click = false;//turn manually off
			bool_endfull = outofspaceend.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 12, bool_endfull);
		}
		//
		IG::DrawCheckbox(true, &focusfirstnpc);
		if (focusfirstnpc.return_click) {
			focusfirstnpc.return_click = false;//turn manually off
			bool_focusonnpc = focusfirstnpc.box_ticked;
			IG::SaveIntSetting(setting_folder, setting_list, 13, bool_focusonnpc);
		}


		Sleep(50);
	}
}

//make a loopy script
void CombatTesting() {

	int foods_failcount = 0;
	combat_check = 0;


	//get//check abilities, those that give no feedback/cooldown
	Abilitybar DeflectMagic = MEX::GetABs_name("Deflect Magic");
	if (DeflectMagic.id == 0) { DebugImGui.Debugtext_add("Ability: Deflect Magic : not found"); }
	Abilitybar DeflectMelee = MEX::GetABs_name("Deflect Melee");
	if (DeflectMelee.id == 0) { DebugImGui.Debugtext_add("Ability: Deflect Melee :not found"); }
	Abilitybar DeflectMissiles = MEX::GetABs_name("Deflect Missiles");
	if (DeflectMissiles.id == 0) { DebugImGui.Debugtext_add("Ability: Deflect Missiles :not found"); }
	Abilitybar SoulSplit = MEX::GetABs_name("Soul Split");
	if (SoulSplit.id == 0) { DebugImGui.Debugtext_add("Ability: Soul Split :not found"); }
	Abilitybar EatFood = MEX::GetABs_name("Eat Food");
	if (EatFood.id == 0) { DebugImGui.Debugtext_add("Ability: Eat Food :not found"); }

	int target_healt = 0;
	fake_mouse_do = true;

	thread bot2(ScriptPaint);
	bot2.detach();
	Sleep(1000);

	while (LoopyLoop) {

		//randoms
		DO::RandomEvents();


		if (ME::Math_RandomNumber(1000) > 985) {
			ME::PIdle1();
		}
		if (ME::Math_RandomNumber(1000) > 990) {
			ME::PIdle2();
		}

		//do inventory managment
		if (1) {
			if (bool_note && !Note_ids_int.empty()) {
				int notepaper = 30372;
				if (ME::InvItemFound(notepaper)) {
					if (ME::Math_ValueEquals(notepaper, Note_ids_int)) {
						DebugImGui.Debugtext_add("U fool, world is gona implode nows;S");
						bool_note = false;
						LoopyLoop = false;
						continue;
					}
					if (ME::InvItemFound(Note_ids_int)) {
						ScripCuRunning1 = "Certs";  
						DO::DoAction_Inventory(Note_ids_int, 0, 0, OFF_ACT::Bladed_interface_route);
						ME::RandomSleep2(40, 550, 900);
						DO::DoAction_Inventory(notepaper, 0, 0, OFF_ACT::GeneralInterface_route1);
						ME::RandomSleep2(500, 550, 900);
					}
				}
				else {
					failure_rate++;
					if (failure_rate > 5) { failure_rate = 0; bool_note = false; }
					DebugImGui.Debugtext_add("No notepaper");
				}
			}
			if (bool_alch && !Alch_ids_int.empty()) {
				if (ME::InvItemFound(Alch_ids_int)) {
					if (ME::VB_FindPSettinOrder(4).state == 1024) {
						Abilitybar HLA = MEX::GetABs_name("High Level Alchemy");
						if (HLA.enabled) {
							ScripCuRunning1 = "Alch";
							DO::DoAction_Ability_Direct(HLA, 0, OFF_ACT::Bladed_interface_route);
							ME::RandomSleep2(40, 550, 900);
							DO::DoAction_Inventory(Alch_ids_int, 0, 0, OFF_ACT::GeneralInterface_route1);
							ME::RandomSleep2(700, 888, 1866);
						}
						else {
							failure_rate++;
							if (failure_rate > 5) { failure_rate = 0; bool_alch = false; }
							DebugImGui.Debugtext_add("No alch");
						}
					}
					else {
						failure_rate++;
						if (failure_rate > 5) { failure_rate = 0; bool_alch = false; }
						DebugImGui.Debugtext_add("Wrong spellbook");
					}
				}
			}
			if (bool_excalibur) {
				int eexalibur = 14632;//2
				int eexalibur_aug = 36619;//2
				int ancientshard = 43358;//1
				if (Health_precentage < 50 + ME::Math_RandomNumber(30)) {
					if (ME::InvItemFound(eexalibur)) {
						Bbar be = MEX::DeBuffbar_GetIDstatus(eexalibur, false);
						if (!be.found) {
							ScripCuRunning1 = "Ex";
							DO::DoAction_Inventory(eexalibur, 0, 2, OFF_ACT::GeneralInterface_route);
							ME::RandomSleep2(1060, 550, 900);
							continue;
						}
					}
				}
				if (Pray_precentage < 50 + ME::Math_RandomNumber(30)) {
					if (ME::InvItemFound(ancientshard)) {
						Bbar ba = MEX::DeBuffbar_GetIDstatus(ancientshard, false);
						if (!ba.found) {
							ScripCuRunning1 = "Ancient";
							DO::DoAction_Inventory(ancientshard, 43358, 1, OFF_ACT::GeneralInterface_route);
							ME::RandomSleep2(1060, 550, 900);
							continue;
						}
					}
				}
			}
			if (bool_drop && !Drop_ids_int.empty()) {
				if (ME::InvItemFound(Drop_ids_int)) {
					ScripCuRunning1 = "Drop";
					DO::DoAction_Inventory(Drop_ids_int, 0, 8, OFF_ACT::GeneralInterface_route2);
					ME::RandomSleep2(700, 888, 1866);
					if (ME::Compare2874Status(28)) {
						DO::DoAction_Interface(0xffffffff, 0xffffffff, 0, 1183, 5, -1, 2944);
						ME::RandomSleep2(500, 550, 900);
					}
				}
			}
		}

		//do prayer managment
		if (Use_quickprays) {
			if (Pray_precentage > 30 + ME::Math_RandomNumber(30)) {
				if (!ME::GetQuickPray()) {
					ScripCuRunning1 = "Q pray";
					DO::DoAction_Button_QP();
					ME::RandomSleep2(560, 550, 900);
				}
			}
			else {
				Abilitybar SR_potion = MEX::GetABs_name("Super restore potion");				
				if (SR_potion.enabled) {
					DO::DoAction_Ability_Direct(SR_potion, 1, OFF_ACT::GeneralInterface_route);
					ScripCuRunning1 = "Drink prayer pot";
					ME::RandomSleep2(1060, 550, 900);
					continue;
				}
				Abilitybar Prayer_potion = MEX::GetABs_name("Prayer potion");
				if (Prayer_potion.enabled) {
					DO::DoAction_Ability_Direct(Prayer_potion, 1, OFF_ACT::GeneralInterface_route);
					ScripCuRunning1 = "Drink prayer pot";
					ME::RandomSleep2(1060, 550, 900);
					continue;
				}
				failure_rate++;
				if (failure_rate > 5) { failure_rate = 0; Silent_Use_quickprays = true; }
				if (!Silent_Use_quickprays) {
					DebugImGui.Debugtext_add("No pray pots");
				}
			}
		}

		//do inv space
		if (bool_eatfoodstomakespace) {
			if (ME::InvFull_()) {
				bool done = false;
				Abilitybar ab = MEX::GetABs_name("Eat Food");
				if (ab.enabled) {
					ScripCuRunning1 = "Eatspace";
					DO::DoAction_Ability_Direct(ab, 1, OFF_ACT::GeneralInterface_route);
					done = true;
					DebugImGui.Debugtext_add("Eatspace");
					ME::RandomSleep2(860, 550, 900);
				}
				if (!done) {
					Abilitybar SR_potion = MEX::GetABs_name("Super restore potion");
					if (SR_potion.enabled) {
						DO::DoAction_Ability_Direct(SR_potion, 1, OFF_ACT::GeneralInterface_route);
						done = true;
						ScripCuRunning1 = "Drink prayer pot space";
						ME::RandomSleep2(1060, 550, 900);
					}
					if (!done) {
						Abilitybar Prayer_potion = MEX::GetABs_name("Prayer potion");
						if (Prayer_potion.enabled) {
							DO::DoAction_Ability_Direct(Prayer_potion, 1, OFF_ACT::GeneralInterface_route);
							done = true;
							ScripCuRunning1 = "Drink prayer pot space";
							ME::RandomSleep2(1060, 550, 900);
						}
					}
				}
				if (!done) {
					Check_space++;
				}
				else {
					Check_space = 0;
				}
				if (bool_endfull && Check_space > 3) {
					vector<int>Ground_List{};
					if (Pickup_D) {
						Ground_List = ask_list_Gr_ids_int;
						Ground_List.insert(Ground_List.end(), Rare_D.begin(), Rare_D.end());
					}
					else {
						Ground_List = ask_list_Gr_ids_int;
					}
					if (DO::DoAction_Loot_w(Ground_List, 50, starttile, area)) {
						ScripCuRunning1 = "Loot";
					}
					//LoopyLoop = false;
					//DO::DoAction_Ability("Retreat Teleport", 1, OFF_ACT::GeneralInterface_route);
					//ME::RandomSleep2(8650, 550, 900);
					//DO::DoAction_Logout_mini();
					//ME::RandomSleep2(650, 550, 900);
					//DO::DoAction_then_lobby();
					continue;
				}
			}
		}

		//do healing
		if (Eat_foods) {
			if (Health_precentage < 35 + ME::Math_RandomNumber(35)) {
				Abilitybar ab = MEX::GetABs_name("Eat Food");
				if (ab.id == 0) {
					DebugImGui.Debugtext_add("No Eat Food: at ability bar?");
				}
				if (ab.enabled) {
					ScripCuRunning1 = "Eat";
					DO::DoAction_Ability_Direct(ab, 1, OFF_ACT::GeneralInterface_route);
					DebugImGui.Debugtext_add("Eat");
					ME::RandomSleep2(860, 550, 900);
				}
				else {
					failure_rate++;
					if (failure_rate > 5) { failure_rate = 0; Silent_Eat_foods = true; }
					if (!Silent_Eat_foods) {
						DebugImGui.Debugtext_add("No foods");
					}
				}
			}
		}

		//monitor
		bool Healthok = false;
		if (Health_precentage < 30) {
			ScripCuRunning1 = "Teleport?";
			console_text << "Healtcritical:teleport?: "<< Health_precentage << endl;
			DebugImGui.Debugtext_addstream();
			vector<int>Ground_List{};
			if (Pickup_D) {
				Ground_List = ask_list_Gr_ids_int;
				Ground_List.insert(Ground_List.end(), Rare_D.begin(), Rare_D.end());
			}
			else {
				Ground_List = ask_list_Gr_ids_int;
			}
			if (DO::DoAction_Loot_w(Ground_List, 50, starttile, area)) {
				ScripCuRunning1 = "Loot";
			}
			//LoopyLoop = false;
			//DO::DoAction_Ability("Retreat Teleport", 1, OFF_ACT::GeneralInterface_route);
			//ME::RandomSleep2(8650, 550, 900);
			//DO::DoAction_Logout_mini();
			//ME::RandomSleep2(650, 550, 900);
			//DO::DoAction_then_lobby();
			continue;
		}
		else {
			Healthok = true;
		}

		bool lootok = false;
		if (bool_pickup) {//do ground items
			vector<int>Ground_List{};
			if (Pickup_D) {			
				Ground_List = ask_list_Gr_ids_int;				
				Ground_List.insert(Ground_List.end(), Rare_D.begin(), Rare_D.end());
			}
			else {
				Ground_List = ask_list_Gr_ids_int;
			}
			if (DO::DoAction_Loot_w(Ground_List, 50, starttile, area)) {
				ScripCuRunning1 = "Loot";
				ME::RandomSleep2(1560, 550, 900);
				MEX::WaitUntilMovingEnds();
			}
			else {
				lootok = true;
			}
		}
		else {
			lootok = true;
		}

		bool target = true;
		if (IgnoreInCombat) {
			if (ME::GetTargetHealth() == 0) {
				target = false;
			}
		}

		//check if combat is actually happening
		if (ME::LocalPlayer_IsInCombat_()) {
			combat_check = 0;
		}
		else {
			combat_check++;
		}
		if (combat_check > 200) {
			DebugImGui.Debugtext_add("CombatTesting:no combat?");
			vector<int>Ground_List{};
			if (Pickup_D) {
				Ground_List = ask_list_Gr_ids_int;
				Ground_List.insert(Ground_List.end(), Rare_D.begin(), Rare_D.end());
			}
			else {
				Ground_List = ask_list_Gr_ids_int;
			}
			if (DO::DoAction_Loot_w(Ground_List, 50, starttile, area)) {
				ScripCuRunning1 = "Loot";
			}
			//LoopyLoop = false;
			//DO::DoAction_Ability("Retreat Teleport", 1, OFF_ACT::GeneralInterface_route);
			//ME::RandomSleep2(8650, 550, 900);
			//DO::DoAction_Logout_mini();
			//ME::RandomSleep2(650, 550, 900);
			//DO::DoAction_then_lobby();
			continue;
		}

		if (!ask_npcready.empty()) {
			bool done = false;
			//on first, switch in combat if it is not on right target
			if (bool_focusonnpc) {
				if (ask_npcready.front().size() > 1 && ME::LocalPlayer_IsInCombat_()) {
					if (ask_npcready.front().compare(ME::Local_PlayerInterActingWith_str(5)) != 0) {
						if (DO::DoAction_NPC_str(42, OFF_ACT::AttackNPC_route, { ask_npcready.front() }, area, 1, 99)) {
							console_text << "CombatTesting:switch to: " << ask_npcready.front() << endl;
							DebugImGui.Debugtext_addstream();
							done = true;
							ME::RandomSleep2(150, 550, 900);
						}
					}
				}
			}

			if (bool_attack && lootok && Healthok && !done) {//do npc
				if (!ME::LocalPlayer_IsInCombat_() || !target) {
					if (DO::DoAction_NPC_str(42, OFF_ACT::AttackNPC_route, ask_npcready, area, 1, 99)) {
						ME::RandomSleep2(150, 550, 900);
					}
				}
			}
		}
		ME::RandomSleep2(150, 550, 900);
	}
}
