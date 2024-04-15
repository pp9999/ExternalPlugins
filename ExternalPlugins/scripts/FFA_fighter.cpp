#pragma once
#include "API.h"
#include "ScriptLoader.h"
#include <algorithm>
#include <thread>
#include <filesystem>

using namespace std;

static IG_answer AttackBack{};
static IG_answer WalkingToPlayers{};
static IG_answer CheckforTargetthenAtt{};
static IG_answer CheckforCombatthenAtt{};
static IG_answer CheckforOtherPlayerCombatthenAtt{};
static IG_answer WalkBacktoZone{};

static int SelectArea() {
	string Cselect = MEX::ScriptDialogWindow2("FFA_fighter", { "StartPortal", "StartMultiNorth","West","South Pond","North Pond" }).Name;

	if (Cselect.compare("StartPortal") == 0) {
		std::cout << Cselect << endl;
		ScripCuRunning0 = Cselect;
		return 1;
	}
	if (Cselect.compare("StartMultiNorth") == 0) {
		std::cout << Cselect << endl;
		ScripCuRunning0 = Cselect;
		return 2;
	}
	if (Cselect.compare("South Pond") == 0) {
		std::cout << Cselect << endl;
		ScripCuRunning0 = Cselect;
		return 3;
	}
	if (Cselect.compare("West") == 0) {
		std::cout << Cselect << endl;
		ScripCuRunning0 = Cselect;
		return 4;
	}
	if (Cselect.compare("North Pond") == 0) {
		std::cout << Cselect << endl;
		ScripCuRunning0 = Cselect;
		return 5;
	}
	return 0;
}

//any match
static bool quickcompare(vector <string> Players, string Player) {
	for (string pl : Players) {
		if (pl.compare(Player) == 0) {
			return true;
		}
	}
	return false;
}

//custom filter
static AllObject Playerfilter(vector <string> Players) {
	int xstart = 2756;
	int xend = 2875;
	int ystart = 5512;
	int yend = 5627;
	int multistart_y = 5535, multiend_y = 5627;
	vector<AllObject> players_list;
	vector<AllObject> far_players_list;
	vector<AllObject> AllMain = ME::ReadAllObjectsArray({ 2 }, { -1 }, Players);
	FFPOINT Localpl_FF = ME::PlayerCoordfloat();
	for (int i = 0; i < AllMain.size(); i++) {
		if (ME::InArea2(xstart, xend, static_cast<int>(AllMain[i].Tile_XYZ.x), ystart, yend, static_cast<int>(AllMain[i].Tile_XYZ.y))) {//player is in certain area
			if (AllMain[i].Name.compare(LocalPlayerName) == 0) { continue; }//ignore localplayer
			if (quickcompare(Players, AllMain[i].Name)) {
				if (ME::Math_DistanceF(Localpl_FF, AllMain[i].Tile_XYZ) < 20.f) {//player is within attacking distance
					bool inmulti = false;
					if (ME::PInArea2(xstart, xend, multistart_y, multiend_y)) {//check our player
						if (ME::InArea2(xstart, xend, static_cast<int>(AllMain[i].Tile_XYZ.x), ystart, yend, static_cast<int>(AllMain[i].Tile_XYZ.y))) {//check other player
							inmulti = true;
						}
					}
					//get stances
					int stance = ME::Readint(AllMain[i].Mem + 0xF08);
					if (CheckforOtherPlayerCombatthenAtt.box_ticked) {
						if (stance > 2686 && stance < 2694) {
							continue;
						}
					}
					if (!(stance > 2686 && stance < 2694) || inmulti) {
						players_list.push_back(AllMain[i]);
					}
				}
				else {
					//too far, for walking
					far_players_list.push_back(AllMain[i]);
				}
			}
		}
	}
	if (!players_list.empty()) {
		sort(players_list.begin(), players_list.end(), ME::Math_Compare_AllObject_dist_smallest);
		return players_list.front();
	}
	if (!far_players_list.empty() && WalkingToPlayers.box_ticked) {
		//ignore this if player is allready moving
		if (!ME::ReadPlayerMovin2()) {
			sort(far_players_list.begin(), far_players_list.end(), ME::Math_Compare_AllObject_dist_smallest);
			//instead start walking towards our target
			cout << "Walk to: " + far_players_list.front().Name << endl;
			thread Walkto(DO::DoAction_WalkerF, far_players_list.front().Tile_XYZ, 50);
			Walkto.detach();
			//do 1 step and break
			Sleep(600);
			WalkingBrake = true;
		}
	}
	return{};
}
void antiIdleTask_FFA() {
	// Init timer
	time_t lastIdleCheck = time(NULL);
	// Time delay between 2 and 4 minutes
	int nextTimeDelay = rand() % ((240 + 1) - 120) + 120;

	cout << "[IdleHandler] Performing anti idle action" << endl;
	ME::PIdle2();
	ME::RandomSleep2(100, 50, 100);
	auto calcedGap = lastIdleCheck + nextTimeDelay;
	cout << "[IdleHandler] Next anti idle action at " << put_time(std::localtime(&calcedGap), "%d-%m-%Y %H-%M-%S") << endl;

	while (LoopyLoop && !Endall) {
		if ((lastIdleCheck + nextTimeDelay) < time(NULL)) {
			// Reset variables to next random delay and update timer
			lastIdleCheck = time(NULL);
			nextTimeDelay = rand() % ((240 + 1) - 120) + 120;

			cout << "[IdleHandler] Performing anti idle action" << endl;
			ME::PIdle2();
			ME::RandomSleep2(100, 50, 100);
			calcedGap = lastIdleCheck + nextTimeDelay;
			cout << "[IdleHandler] Next anti idle action at " << put_time(std::localtime(&calcedGap), "%d-%m-%Y %H-%M-%S") << endl;
		}
		ME::RandomSleep2(400, 150, 250);
	}
}
//v2
void FFA_fighter() {

	Doaction_paint = true;

	AttackBack.box_name = "AttackBack";
	AttackBack.box_start = { 20,40,0 };
	AttackBack.box_size = { 150,20,0 };
	AttackBack.box_ticked = true;
	IG::DrawCheckbox(true, &AttackBack);
	WalkingToPlayers.box_name = "WalkingToPlayers";
	WalkingToPlayers.box_start = { 20,60,0 };
	WalkingToPlayers.box_size = { 160,20,0 };
	WalkingToPlayers.box_ticked = false;
	IG::DrawCheckbox(true, &WalkingToPlayers);
	CheckforTargetthenAtt.box_name = "CheckforTargetthenAtt";
	CheckforTargetthenAtt.box_start = { 20,80,0 };
	CheckforTargetthenAtt.box_size = { 180,20,0 };
	CheckforTargetthenAtt.box_ticked = true;
	IG::DrawCheckbox(true, &CheckforTargetthenAtt);
	CheckforCombatthenAtt.box_name = "CheckforCombatthenAtt";
	CheckforCombatthenAtt.box_start = { 20,100,0 };
	CheckforCombatthenAtt.box_size = { 180,20,0 };
	CheckforCombatthenAtt.box_ticked = false;
	IG::DrawCheckbox(true, &CheckforCombatthenAtt);
	CheckforOtherPlayerCombatthenAtt.box_name = "CheckforOtherPlayerCombatthenAtt";
	CheckforOtherPlayerCombatthenAtt.box_start = { 20,120,0 };
	CheckforOtherPlayerCombatthenAtt.box_size = { 260,20,0 };
	CheckforOtherPlayerCombatthenAtt.box_ticked = false;
	IG::DrawCheckbox(true, &CheckforOtherPlayerCombatthenAtt);
	WalkBacktoZone.box_name = "WalkBacktoZone";
	WalkBacktoZone.box_start = { 20,140,0 };
	WalkBacktoZone.box_size = { 160,20,0 };
	WalkBacktoZone.box_ticked = true;
	IG::DrawCheckbox(true, &WalkBacktoZone);



	vector <string> Players = {  };
	vector <NameData> Pdata = MEX::ScriptDialogWindow_input("Targets", 0, 2, "player_names");
	for (NameData P : Pdata) {
		Players.push_back(P.Name);
	}

	thread antiIdleThread = thread(antiIdleTask_FFA);
	int SelectedSpin = SelectArea();
	while (LoopyLoop) {

		if (SelectedSpin == 1) { // portal
			if (ME::PInArea2(2980, 3003, 9675, 9690)) {
				ScripCuRunning1 = "In lobby. Entering FFA portal.";
				//ME::FindAObj({ 38698 }, 25, 5, 1, 0, "Enter");
				DO::DoAction_Object_string_r(0x39, OFF_ACT::GeneralObject_route0, { "Free-for-all (safe)" }, 50, { 2989,9684,0 }, 3.f);
				ME::RandomSleep2(500, 2250, 12000);
			}
			else {
				if (ME::PInArea2(2808, 2832, 5509, 5511)) {
					ScripCuRunning1 = "In safezone. Moving out.";
					if (!ME::GetQuickPray()) {
						DO::DoAction_Interface(0xffffffff, 0xffffffff, 1, 1430, 16, -1, OFF_ACT::GeneralInterface_route);
					}
					DO::DoAction_Tile({ 2812 + ME::Math_RandomNumber(5), 5512 + ME::Math_RandomNumber(8),0 });
					ME::RandomSleep2(500, 2250, 12000);
				}
				if (ME::PInArea2(2798, 2825, 5512, 5535)) {
					ScripCuRunning1 = "Not safe area.";
					if (AttackBack.box_ticked) {
						AllObject player = ME::OtherPlayerIsInteractingWithVSLocal();
						if (player.TileX > 0) {
							ScripCuRunning1 = "Attack back";
							DO::DoAction_VS_Player_action_Direct(player, OFF_ACT::Vs_player_attack_route);
						}
					}
					if (CheckforTargetthenAtt.box_ticked) {
						if (ME::GetTargetHealth() != 0) {
							continue;
						}
					}
					if (CheckforCombatthenAtt.box_ticked) {
						if (ME::LocalPlayer_IsInCombat_()) {
							continue;
						}
					}
					ScripCuRunning1 = "Attack other players.";
					AllObject player = Playerfilter(Players);
					if (player.TileX > 0) {
						cout << "Target is: " << player.Name << endl;
						ScripCuRunning1 = "Target is:";
						ScripCuRunning1.append(player.Name);
						DO::DoAction_VS_Player_action_Direct(player, OFF_ACT::Vs_player_attack_route);
					}
					else {
						cout << "NO Target!" << endl;
						ScripCuRunning1 = "NO Target!";
					}
					ME::RandomSleep2(2500, 4250, 12000);
				}
				else {
					if (WalkBacktoZone.box_ticked) {
						DO::DoAction_Tile({ 2812 + ME::Math_RandomNumber(5), 5512 + ME::Math_RandomNumber(8),0 });
						ME::RandomSleep2(500, 2250, 12000);
					}
				}
				ScripCuRunning1 = "Waiting...";
			}
		}

		if (SelectedSpin == 2) { // multinorth
			if (ME::PInArea2(2980, 3003, 9675, 9690)) {
				ScripCuRunning1 = "In lobby. Entering FFA portal.";
				DO::DoAction_Object_string_r(0x39, OFF_ACT::GeneralObject_route0, { "Free-for-all (safe)" }, 50, { 2989,9684,0 }, 3.f);
				ME::RandomSleep2(500, 2250, 12000);
			}
			else {
				if (ME::PInArea2(2808, 2832, 5509, 5511)) {
					ScripCuRunning1 = "In safezone. Moving out.";
					if (!ME::GetQuickPray()) {
						DO::DoAction_Interface(0xffffffff, 0xffffffff, 1, 1430, 16, -1, OFF_ACT::GeneralInterface_route);
					}
					DO::DoAction_Tile({ 2814 + ME::Math_RandomNumber(2), 5540 + ME::Math_RandomNumber(3),0 });
					ME::RandomSleep2(500, 2250, 12000);
				}
				if (ME::PInArea2(2809, 2816, 5540, 5550)) {
					ScripCuRunning1 = "Not safe area.";
					if (AttackBack.box_ticked) {
						AllObject player = ME::OtherPlayerIsInteractingWithVSLocal();
						if (player.TileX > 0) {
							ScripCuRunning1 = "Attack back";
							DO::DoAction_VS_Player_action_Direct(player, OFF_ACT::Vs_player_attack_route);
							ME::RandomSleep2(500, 2250, 12000);
						}
					}
					if (CheckforTargetthenAtt.box_ticked) {
						if (ME::GetTargetHealth() != 0) {
							continue;
						}
					}
					if (CheckforCombatthenAtt.box_ticked) {
						if (ME::LocalPlayer_IsInCombat_()) {
							continue;
						}
					}
					ScripCuRunning1 = "Attack other players.";
					AllObject player = Playerfilter(Players);
					if (player.TileX > 0) {
						cout << "Target is: " << player.Name << endl;
						ScripCuRunning1 = "Target is:";
						ScripCuRunning1.append(player.Name);
						DO::DoAction_VS_Player_action_Direct(player, OFF_ACT::Vs_player_attack_route);
					}
					else {
						cout << "NO Target!" << endl;
						ScripCuRunning1 = "NO Target!";
					}
					ME::RandomSleep2(2500, 4250, 12000);
				}
				else {
					if (WalkBacktoZone.box_ticked) {
						DO::DoAction_Tile({ 2814 + ME::Math_RandomNumber(2), 5540 + ME::Math_RandomNumber(3),0 });
						ME::RandomSleep2(500, 2250, 12000);
					}
				}
				ScripCuRunning1 = "Waiting...";
			}
		}

		if (SelectedSpin == 3) { // south pond
			if (ME::PInArea2(2980, 3003, 9675, 9690)) {
				ScripCuRunning1 = "In lobby. Entering FFA portal.";
				DO::DoAction_Object_string_r(0x39, OFF_ACT::GeneralObject_route0, { "Free-for-all (safe)" }, 50, { 2989,9684,0 }, 3.f);
				ME::RandomSleep2(500, 2250, 12000);
			}
			else {
				if (ME::PInArea2(2808, 2832, 5509, 5511)) {
					ScripCuRunning1 = "In safezone. Moving out.";
					if (!ME::GetQuickPray()) {
						DO::DoAction_Interface(0xffffffff, 0xffffffff, 1, 1430, 16, -1, OFF_ACT::GeneralInterface_route);
						ME::RandomSleep2(500, 2250, 12000);
					}
					DO::DoAction_Tile({ 2829 + ME::Math_RandomNumber(5), 5512 + ME::Math_RandomNumber(8),0 });
					ME::RandomSleep2(500, 2250, 12000);
				}
				if (ME::PInArea2(2829, 2835, 5512, 5518)) {
					ScripCuRunning1 = "Not safe area.";
					if (AttackBack.box_ticked) {
						AllObject player = ME::OtherPlayerIsInteractingWithVSLocal();
						if (player.TileX > 0) {
							ScripCuRunning1 = "Attack back";
							DO::DoAction_VS_Player_action_Direct(player, OFF_ACT::Vs_player_attack_route);
						}
					}
					if (CheckforTargetthenAtt.box_ticked) {
						if (ME::GetTargetHealth() != 0) {
							continue;
						}
					}
					if (CheckforCombatthenAtt.box_ticked) {
						if (ME::LocalPlayer_IsInCombat_()) {
							continue;
						}
					}
					ScripCuRunning1 = "Attack other players.";
					AllObject player = Playerfilter(Players);
					if (player.TileX > 0) {
						cout << "Target is: " << player.Name << endl;
						ScripCuRunning1 = "Target is:";
						ScripCuRunning1.append(player.Name);
						DO::DoAction_VS_Player_action_Direct(player, OFF_ACT::Vs_player_attack_route);
					}
					else {
						cout << "NO Target!" << endl;
						ScripCuRunning1 = "NO Target!";
					}
					ME::RandomSleep2(2500, 4250, 12000);
				}
				else {
					if (WalkBacktoZone.box_ticked) {
						DO::DoAction_Tile({ 2829 + ME::Math_RandomNumber(5), 5512 + ME::Math_RandomNumber(8),0 });
						ME::RandomSleep2(500, 2250, 12000);
					}
				}
				ScripCuRunning1 = "Waiting...";
			}
		}

		if (SelectedSpin == 4) { // start west
			if (ME::PInArea2(2980, 3003, 9675, 9690)) {
				ScripCuRunning1 = "In lobby. Entering FFA portal.";
				DO::DoAction_Object_string_r(0x39, OFF_ACT::GeneralObject_route0, { "Free-for-all (safe)" }, 50, { 2989,9684,0 }, 3.f);
				ME::RandomSleep2(500, 2250, 12000);
			}
			else {
				if (ME::PInArea2(2808, 2832, 5509, 5511)) {
					ScripCuRunning1 = "In safezone. Moving out.";
					if (!ME::GetQuickPray()) {
						DO::DoAction_Interface(0xffffffff, 0xffffffff, 1, 1430, 16, -1, OFF_ACT::GeneralInterface_route);
					}
					DO::DoAction_Tile({ 2790 + ME::Math_RandomNumber(5), 5512 + ME::Math_RandomNumber(4),0 });
					ME::RandomSleep2(500, 2250, 12000);
				}
				if (ME::PInArea2(2780, 2805, 5512, 5520)) {
					ScripCuRunning1 = "Not safe area.";
					if (AttackBack.box_ticked) {
						AllObject player = ME::OtherPlayerIsInteractingWithVSLocal();
						if (player.TileX > 0) {
							ScripCuRunning1 = "Attack back";
							DO::DoAction_VS_Player_action_Direct(player, OFF_ACT::Vs_player_attack_route);
							ME::RandomSleep2(500, 2250, 12000);
						}
					}
					if (CheckforTargetthenAtt.box_ticked) {
						if (ME::GetTargetHealth() != 0) {
							continue;
						}
					}
					if (CheckforCombatthenAtt.box_ticked) {
						if (ME::LocalPlayer_IsInCombat_()) {
							continue;
						}
					}
					ScripCuRunning1 = "Attack other players.";
					AllObject player = Playerfilter(Players);
					if (player.TileX > 0) {
						cout << "Target is: " << player.Name << endl;
						ScripCuRunning1 = "Target is:";
						ScripCuRunning1.append(player.Name);
						DO::DoAction_VS_Player_action_Direct(player, OFF_ACT::Vs_player_attack_route);
					}
					else {
						cout << "NO Target!" << endl;
						ScripCuRunning1 = "NO Target!";
					}
					ME::RandomSleep2(2500, 4250, 12000);
				}
				else {
					if (WalkBacktoZone.box_ticked) {
						DO::DoAction_Tile({ 2790 + ME::Math_RandomNumber(5), 5512 + ME::Math_RandomNumber(4),0 });
						ME::RandomSleep2(500, 2250, 12000);
					}
				}
				ScripCuRunning1 = "Waiting...";
			}
		}

		if (SelectedSpin == 5) // North Pond
		{
			if (ME::PInArea2(2980, 3003, 9675, 9690)) {
				ScripCuRunning1 = "In lobby. Entering FFA portal.";
				DO::DoAction_Object_string_r(0x39, OFF_ACT::GeneralObject_route0, { "Free-for-all (safe)" }, 50, { 2989,9684,0 }, 3.f);
				ME::RandomSleep2(500, 2250, 12000);
			}
			else {
				if (ME::PInArea2(2808, 2832, 5509, 5511)) {
					ScripCuRunning1 = "In safezone. Moving out.";
					if (!ME::GetQuickPray()) {
						DO::DoAction_Interface(0xffffffff, 0xffffffff, 1, 1430, 16, -1, OFF_ACT::GeneralInterface_route);
					}
					DO::DoAction_Tile({ 2834 + ME::Math_RandomNumber(5), 5540 + ME::Math_RandomNumber(8),0 }); // change this
					ME::RandomSleep2(500, 2250, 12000);
				}
				if (ME::PInArea2(2819, 2845, 5538, 5560)) {// change this 
					ScripCuRunning1 = "Not safe area.";
					if (AttackBack.box_ticked) {
						AllObject player = ME::OtherPlayerIsInteractingWithVSLocal();
						if (player.TileX > 0) {
							ScripCuRunning1 = "Attack back";
							DO::DoAction_VS_Player_action_Direct(player, OFF_ACT::Vs_player_attack_route);
							ME::RandomSleep2(500, 2250, 12000);
						}
					}
					if (CheckforTargetthenAtt.box_ticked) {
						if (ME::GetTargetHealth() != 0) {
							continue;
						}
					}
					if (CheckforCombatthenAtt.box_ticked) {
						if (ME::LocalPlayer_IsInCombat_()) {
							continue;
						}
					}
					ScripCuRunning1 = "Attack other players.";
					AllObject player = Playerfilter(Players);
					if (player.TileX > 0) {
						cout << "Target is: " << player.Name << endl;
						ScripCuRunning1 = "Target is:";
						ScripCuRunning1.append(player.Name);
						DO::DoAction_VS_Player_action_Direct(player, OFF_ACT::Vs_player_attack_route);
					}
					else {
						cout << "NO Target!" << endl;
						ScripCuRunning1 = "NO Target!";
					}
					ME::RandomSleep2(2500, 4250, 12000);
				}
				else {
					if (WalkBacktoZone.box_ticked) {
						DO::DoAction_Tile({ 2834 + ME::Math_RandomNumber(5), 5540 + ME::Math_RandomNumber(8),0 });
						ME::RandomSleep2(500, 2250, 12000);
					}
				}
				ScripCuRunning1 = "Waiting...";
			}
		}
		ME::RandomSleep2(1000, 3250, 12000);
	}
	antiIdleThread.join();
}
