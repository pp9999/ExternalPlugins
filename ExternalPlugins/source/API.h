#pragma once
#include <Windows.h>
#include <synchapi.h>
#include <vector>
#include <bitset>
#include <functional>

#include "Structs.h"
//#include "RenderEvent.h"
#include "imgui.h"

using namespace std;

#ifdef LIBRARY_EXPORTS
#    define LIBRARY_API extern "C++" __declspec(dllexport)
#else
#    define LIBRARY_API extern "C++" __declspec(dllimport)
#endif

//Extra exports
//Main loop
LIBRARY_API bool LoopyLoop;
//Unloads everything and closes
LIBRARY_API bool Endall;
//Text displayed in the ME window
LIBRARY_API std::string ScripCuRunning1;
LIBRARY_API std::string ScripCuRunning2;
//LIBRARY_API bool DrawLogs;
//LIBRARY_API bool DrawTrackedSkills;
LIBRARY_API bool fake_mouse_do;//generate fake reports


//localPlayer
LIBRARY_API uint64_t LocalPlayer;

//ImGui use the premade function to subscribe a function to the render loop
//or get the context first
LIBRARY_API ImGuiContext* GetImGuiContext();
LIBRARY_API void SubscribeToRenderEvent(std::function<void()> function);

/// <summary>
/// Should only be used for rendering ImGui
/// heavy maffs will make the game chug
/// </summary>
void DrawImGui(std::function<void()> function);

//some general offsets for doaction
namespace OFF_ACT {

	constexpr int GeneralObject_route00 = -80;//use item/action on object like bladed dive
	constexpr int GeneralObject_route0 = 0;//General action on some objects, like bank chest
	constexpr int GeneralObject_route1 = 80;//General action on some objects
	constexpr int GeneralObject_route2 = 160;//General action on some objects
	constexpr int GeneralObject_route3 = 240;//General action on some objects
	constexpr int BladedDiveNPC_route = 1392;//do bladed dive to npc
	constexpr int InteractNPC_route = 1488;//default interact npc 0x29 
	constexpr int AttackNPC_route = 1600;//default attack npc
	constexpr int InteractNPC_route2 = 1696;//second option
	constexpr int InteractNPC_route3 = 1776;//third option
	constexpr int InteractNPC_route4 = 1888;//fourth option
	constexpr int Pickup_route = 2464;//default item pickup
	constexpr int Walk_route = 2752;//walk to tile 
	constexpr int Bladed_interface_route = 2832;//special interface route for bladed dive, familiar attack, use in inventory..items that can actualy do something use GeneralInterface_route
	constexpr int GeneralInterface_Choose_option = 2912;//option in chat box
	constexpr int GeneralInterface_Choose_jewelry = 2912;//teleport jewelry//chat box?
	constexpr int Vs_player_attack_route = 3072;//
	constexpr int GeneralInterface_route = 3808;//General action on lootwindow, also most interfaces
	constexpr int GeneralInterface_route1 = 3888;//note stuff, use item on empty inv spot, use item on item
	constexpr int GeneralInterface_route2 = 4528;//take bob/store bob/drop items
	constexpr int Vs_player_follow_route = 5648;//old
	constexpr int Vs_player_trade_route = 5728;//old
	constexpr int Special_walk_route = 3968;//Bladed dive teleport

};

namespace DO {
	//Push action, move to a tile
	LIBRARY_API bool DoAction_Tile(WPOINT normal_tile);
	//Tile click
	LIBRARY_API bool DoAction_TileF(FFPOINT normal_tile);
	//Push action, do action on object, find by id
	LIBRARY_API bool DoAction_Object(int action, int offset, std::vector<int> obj, int maxdistance);
	//Use detour at DO::action to scoop them,added to Fake_action_call, specifify tile 
	LIBRARY_API bool DoAction_Object(int action, int offset, std::vector<int> obj, int maxdistance, WPOINT tile);
	//
	LIBRARY_API bool DoAction_Object_valid(int action, int offset, std::vector<int> objects, int maxdistance, bool valid = false);
	//
	LIBRARY_API bool DoAction_Object_valid(int action, int offset, std::vector<int> objects, int maxdistance, WPOINT tile, bool valid = false);
	//Use detour on the furthest object
	LIBRARY_API bool DoAction_Object_furthest(int action, int offset, std::vector<int> obj, int maxdistance);
	//Push action, do action on object, find by id, specifify tile and tile_range to look around
	LIBRARY_API bool DoAction_Object_r(int action, int offset, std::vector<int> obj, int maxdistance, WPOINT tile, float tile_range, bool valid = false);
	//Push action, do action on object, matches up id with highlight
	LIBRARY_API bool FindHl(int action, int offset, std::vector<int> obj, int maxdistance, std::vector<int> highlight);
	//Highlight mark up tile. highlight is that sparkle id /mining 7164, arch 7307, localp_dist = howfar player could go/be
	LIBRARY_API bool FindHLvsLocalPlayer(int action, int offset, std::vector<int> obj, int maxdistance, std::vector<int> highlight, float localp_dist = 2.f);
	//Push action, find by object name.
	LIBRARY_API bool DoAction_Object_string(int action, int offset, std::vector<std::string> obj, int maxdistance, bool CheckInteractbool = false);
	//Push action, do action on object, find by string. Look around tile
	LIBRARY_API bool DoAction_Object_string(int action, int offset, std::vector<std::string> obj, int maxdistance, WPOINT tile, bool CheckInteractbool = false);
	//Push action, do action on object, find by string, object size in tiles
	LIBRARY_API bool DoAction_Object_string_r(int action, int offset, std::vector<std::string> obj, int maxdistance, WPOINT tile, float tile_range, bool CheckInteractbool = false);
	//Push action, do action on object, directly sends command to do
	LIBRARY_API bool DoAction_Object_Direct(int action, int offset, AllObject object);
	//Push action, do action on NPC, find by id
	LIBRARY_API bool DoAction_NPC(int action, int offset, std::vector<int> obj, int maxdistance, bool ignore_star = false, int health = 0);
	//Push action, do action on NPC, find by string
	LIBRARY_API bool DoAction_NPC_str(int action, int offset, std::vector<std::string> obj, int maxdistance, bool ignore_star = false, int health = 0);
	//Push action, do action on NPC, find by id, filter by area
	LIBRARY_API bool DoAction_NPC_In_Area(int action, int offset, std::vector<int> obj, int maxdistance, WPOINT bottom_left, WPOINT top_right, bool ignore_star = false, int health = 0);
	//Push action, do action on NPCs, directly sends command to do
	LIBRARY_API bool DoAction_NPC__Direct(int action, int offset, AllObject object);
	//Push action, do action on player, find by name, attack
	LIBRARY_API bool DoAction_VS_Player_Attack(std::vector<std::string> obj, int maxdistance);
	//Push action, do action on player, find by name, extra parameters
	LIBRARY_API bool DoAction_VS_Player_Attack(std::vector<std::string> obj, int maxdistance, bool checkcombat, int xstart, int xend, int ystart, int yend);
	//Push action, do action on player, find by name, trade
	LIBRARY_API bool DoAction_VS_Player_Trade(std::vector<std::string> obj, int maxdistance);
	//Push action, do action on player, find by name, follow
	LIBRARY_API bool DoAction_VS_Player_Follow(std::vector<std::string> obj, int maxdistance);
	//Push action, do action on player, directly sends command to do
	LIBRARY_API bool DoAction_VS_Player_action_Direct(AllObject obj, int offset);
	//Push action, filter
	LIBRARY_API bool DoAction_G_Items(int action, std::vector<int> obj, int maxdistance);
	//tries to get items, at tile
	LIBRARY_API bool DoAction_G_Items(int action, std::vector<int> obj, int maxdistance, WPOINT atTile);
	//tries to get items, look around raw tile and radius
	LIBRARY_API bool DoAction_G_Items_r(int action, std::vector<int> obj, int maxdistance, FFPOINT tile, float radius);
	//tries to get items, look around tile and radius
	LIBRARY_API bool DoAction_G_Items_r_norm(int action, std::vector<int> obj, int maxdistance, FFPOINT tile, float radius);
	//tries to get items, look around raw tile and radius, checks if inventory is full if it is then gets only stacks that exist in inventory
	LIBRARY_API bool DoAction_G_Items_r_normSTACKs(int action, std::vector<int> obj, int maxdistance, FFPOINT tile, float radius);
	//tries to get items, does special action on them
	LIBRARY_API bool DoAction_G_Items(int action, int action_route, std::vector<int> obj, int maxdistance);
	//tries to get items, does special action on them, at tile
	LIBRARY_API bool DoAction_G_Items(int action, int action_route, std::vector<int> obj, int maxdistance, WPOINT atTile);
	//tries to get items, look around raw tile and radius, does special action on them
	LIBRARY_API bool DoAction_G_Items_r(int action, int action_route, std::vector<int> obj, int maxdistance, FFPOINT tile, float radius);
	//Push action, direct, very unhealty to use directly
	LIBRARY_API bool DoAction_G_Items_Direct(int action, int route, AllObject obj);
	//Push action, send commands to window, no safety checks are made
	LIBRARY_API bool DoAction_Interface(int command1, int command2, int command3, int interface1, int interface2, int interface3, int offset, int pixel_x = 0, int pixel_y = 0);
	//Push LootWindow interaction
	LIBRARY_API bool DoAction_Loot_w(std::vector<int> ids, int maxdistance = 20, FFPOINT tile = { 0,0,0 }, float radius = 20.f);
	//Loot window:loot all button
	LIBRARY_API bool DoAction_LootAll_Button();
	//Loot window:closeX
	LIBRARY_API bool DoAction_Loot_w_Close();
	//That mini logout button attached to minimap
	LIBRARY_API bool DoAction_Logout_mini();
	//Pick from popped up menu to lobby
	LIBRARY_API bool DoAction_then_lobby();
	//Push action, send commands to inventory, all safety checked, mostly
	LIBRARY_API bool DoAction_Inventory(int id, int random, int m_action, int offset);
	//first to come by, for making noted items mostly
	LIBRARY_API bool DoAction_Inventory(std::vector<int> ids, int random, int m_action, int offset);
	//use item name
	LIBRARY_API bool DoAction_Inventory(std::string itemname, int random, int m_action, int offset);
	//Push action, send commands to bank, all safety checked, mostly
	LIBRARY_API bool DoAction_Bank(int id, int m_action, int offset);
	//Push action, send commands to bank, all safety checked, mostly
	LIBRARY_API bool DoAction_Bank(std::string itemname, int m_action, int offset);
	//Push action, send commands to bank inventory, all safety checked, mostly
	LIBRARY_API bool DoAction_Bank_Inv(int id, int m_action, int offset);
	//check stuff before
	LIBRARY_API bool DoAction_Ability_check(std::string name, int m_action, int offset, bool checkenabled, bool oncooldown);
	//find it on action bar 0-2,offset from interface debug
	LIBRARY_API bool DoAction_Ability(std::string name, int m_action, int offset);
	//from struct Abilitybar to action
	LIBRARY_API bool DoAction_Ability_Direct(Abilitybar Ab, int m_action, int offset);
	//for bladed dive
	LIBRARY_API bool DoAction_BD_Tile(WPOINT normal_tile);
	//for bladed dive OBJECT
	LIBRARY_API bool DoAction_BD_Obj(int perma_object_id, int maxdistance = 50);
	//for bladed dive NPC
	LIBRARY_API bool DoAction_BD_NPC(int perma_object_id, int maxdistance = 50);
	//for Dive
	LIBRARY_API bool DoAction_Dive_Tile(WPOINT normal_tile);
	//choose chat box option, Whole words/sentences only
	LIBRARY_API bool Dialog_Option(std::string text);
	//Continue dialog
	LIBRARY_API bool Continue_Dialog();
	//find random events, and click them
	LIBRARY_API bool RandomEvents();
	//click specific rand id
	LIBRARY_API bool RandomEvent(int npcid);
	//That heart button,to generate health from adreline
	LIBRARY_API bool DoAction_Button_GH();
	//Auto-retaliate button
	LIBRARY_API bool DoAction_Button_AR();
	//Quick-pray button
	LIBRARY_API bool DoAction_Button_QP();
	//Familiar button, only works if CALL FAMILIAR is first at settings, then other orders fall in line
	/*
	0 - call familiar
	1 - cast special //legendary = no
	2 - attack //legendary = can't
	3 - summon pet
	4 - dismiss
	5 - follower details
	6 - interact
	7 - renew familiar
	8 - give bob
	9 - take bob
	10 - restore points
	*/
	LIBRARY_API bool DoAction_Button_FO(int Possible_order);
	//divide long walks into steps, 
	//WalkingBrake global to stop walking, waitloops = how long to wait between walking clicks
	LIBRARY_API bool DoAction_WalkerW(WPOINT normal_tile, int waitloops = 50);
	//divide long walks into steps, 
	//WalkingBrake global to stop walking
	LIBRARY_API bool DoAction_WalkerF(FFPOINT normal_tile, int waitloops = 50);
	//teleport to lodestone
	LIBRARY_API bool DoAction_Lode(int loc);
	//use strings, part of teleport location will work
	LIBRARY_API bool DoAction_Lode_string(std::string loc_tele);
	//normal spellbook
	LIBRARY_API bool DoAction_Tele(int loc_tele);
	//use strings, part of teleport location will work
	LIBRARY_API bool DoAction_Tele_string(std::string loc_tele);
	//jewelry/items teleports
	LIBRARY_API bool DoAction_Jewelry(std::string itemname, std::string loc_tele, std::string loc_tele2, int item_menu_level = 1, int offset = OFF_ACT::GeneralInterface_route);
	//Spirit tree at Manor Farm
	LIBRARY_API bool DoAction_SpiritTree_string(std::string tele_loc);
	//Spirit tree at Manor Farm
	LIBRARY_API bool DoAction_SpiritTree(int tele_loc);
	//Glider at Al-kharid
	LIBRARY_API bool DoAction_Glider_string(std::string tele_loc);
	//Glider at Al-kharid
	LIBRARY_API bool DoAction_Glider(int tele_loc);
	//use fairy ring
	LIBRARY_API bool DoAction_Fairy(std::string tele_loc);
	//try to walk to the gate, then walk trough
	LIBRARY_API bool Walk_gates(std::vector<GateTileStruct> gatetilesss, int walk_distance);

};

namespace ME {

	//local time
	LIBRARY_API std::tm* PrintLocalTime(bool print);

	//Read All object to ImVec2 pixels
	//LIBRARY_API ImVec2 ReadObjectCoordinatesImVec2(uint64_t mem_location);

	//Read All object to WPOINT pixels
	LIBRARY_API WPOINT ReadObjectCoordinatesWPOINT(uint64_t mem_location);

	//Read All object to FFPOINT pixels
	LIBRARY_API FFPOINT ReadObjectCoordinatesFFPOINT(uint64_t mem_location);

	//Try to determine if object is still there
	LIBRARY_API bool CheckObjectValid(uint64_t mem_local, uint64_t mem_global);

	//Calculation between 2 objects
	LIBRARY_API float Math_DistanceA(AllObject object1, AllObject object2);

	//raw tiles(world pixels) to SCREEN
	LIBRARY_API FFPOINT Math_W2Sv2(FFPOINT entity);

	//raw tiles(world pixels) to SCREEN, return WPOINT
	LIBRARY_API WPOINT Math_W2Sv2W(FFPOINT entity);

	//Calculation between 2 objects, rounded to int
	LIBRARY_API int Math_DistanceRounded(WPOINT object1, WPOINT object2);

	//Calculation between 2 objects
	LIBRARY_API float Math_DistanceW(WPOINT object1, WPOINT object2);

	//Calculation between 2 objects
	LIBRARY_API float Math_DistanceF(FFPOINT object1, FFPOINT object2);

	//Maybe save some arrays
	LIBRARY_API bool SaveFFPOINTs(std::string name, std::vector<FFPOINT> array_points);

	//Maybe save some arrays
	LIBRARY_API std::vector<FFPOINT> LoadFFPOINTs(std::string name);

	//Mouse has to actually move to use this
	LIBRARY_API WPOINT GetTilesUnderCurrentMouse();

	//Mouse has to actually move to use this, norm float
	LIBRARY_API FFPOINT GetTilesUnderCurrentMouseF();

	//normal tiles to pixels on screen, WPOINT in WPOINT out
	LIBRARY_API WPOINT TilesToPixelsWW(WPOINT xy);

	//normal tiles to pixels on screen, float in WPOINT out
	LIBRARY_API WPOINT TilesToPixelsFW(FFPOINT xy);

	//normal tiles to pixels on screen, float in float out
	LIBRARY_API FFPOINT TilesToPixelsFF(FFPOINT xy);

	//normal tiles to pixels on screen, WPOINT in float out
	LIBRARY_API FFPOINT TilesToPixelsWF(WPOINT xy);

	//read as string, constant size
	LIBRARY_API std::string ReadChars(uint64_t SummPointer, int readsize = 250);

	//sorting function
	LIBRARY_API bool Math_comparebigger(ChOpt SummAddress1, ChOpt SummAddress2);

	//
	LIBRARY_API std::vector<uint64_t> GatherIU(uint64_t inputaddr, int offset);

	//
	LIBRARY_API std::vector<uint64_t> GatherIUM(uint64_t inputaddr);

	//
	LIBRARY_API bool ScreenFilter(WPOINT mxy);

	//read as string, constant size, use pointer that points to strings
	LIBRARY_API std::string ReadCharsPointer(uint64_t SummPointer);

	//
	LIBRARY_API bool CheckCoordLimit(int limitx, int limity);

	//
	LIBRARY_API bool CheckCoordLimit2(float limitx, float limity);

	//
	LIBRARY_API bool CheckVisibleLimit(float limitx, float limity);

	//
	LIBRARY_API bool CheckVisibleLimit2(unsigned short limitx, unsigned short limity);

	//make hex string suitable for pattern scan, unknown is wildcard char replacement
	LIBRARY_API std::vector<uint8_t> string_to_pattern(std::string todo, std::string unknown = "CC");

	//
	LIBRARY_API bool PlayerLoggedIn();

	//
	LIBRARY_API bool World_Hopper(int worldtohop);

	//
	LIBRARY_API bool World_Hopper(std::string worldtohop);

	//
	LIBRARY_API bool LoginFunction(std::string name, std::string password, int world);

	//
	LIBRARY_API bool LoginFunction(std::string name, std::string password, std::string world);

	//presses dialog, PostM only
	LIBRARY_API bool Select_Option(std::string text);

	//Gets Interface by string id from I_Madness
	LIBRARY_API InterfaceComp5 GetIbystring(std::string text);

	//Gets Interface name by dynamic
	LIBRARY_API std::string GetINamebyDynamic(InterfaceComp5 ids);

	//Gets Interface by string id from I_Madness,
	LIBRARY_API std::vector<InterfaceComp5> GetIbystringstatic(std::string text);

	//Gets data. x, y, index, 0, memory
	LIBRARY_API InterfaceComp5 GetIMadnessOP(InterfaceComp5 mad);

	//compares text in dialog, if not found returns 0, else returns number to press
	LIBRARY_API char Dialog_Option(std::string text);

	//read what npc is saying
	LIBRARY_API std::string Dialog_Read_NPC();

	//read what player is saying
	LIBRARY_API std::string Dialog_Read_Player();

	//try to find a match in said dialog
	LIBRARY_API bool Dialog_compare_sayd(std::string search_word);

	//check for dialog, part of player talking dialog
	LIBRARY_API bool Check_Dialog_Open();

	//check for continue window NPC or Player
	LIBRARY_API bool Check_continue_Open();

	//check for continue window, part of NPC talking dialog
	LIBRARY_API bool Check_continue_Open_NPC();

	//check for continue window, part of player talking dialog
	LIBRARY_API bool Check_continue_Open_Player();

	// check for continue window, part of talking dialog
	LIBRARY_API bool Check_continue_Open_Server();

	//floor lv localplayer
	LIBRARY_API int GetFloorLv_2();

	//Tile distance from localplayer
	LIBRARY_API float Dist_TFLP(FFPOINT object);

	//
	LIBRARY_API WPOINT InvFindItem(int item, int action);

	//
	LIBRARY_API WPOINT InvFindItem(std::string item, int action);

	//
	LIBRARY_API bool FindGItemBool_(std::vector<int> item);

	//
	LIBRARY_API bool FindGItem_AllBut2(std::vector<int> Except_item, int maxdistance, int accuracy, WPOINT tilespot, int maxdistance2, std::vector<int> items_to_eat);

	//Try all possible locations of text
	LIBRARY_API std::string InterfaceGetALLText(uint64_t base);

	//
	LIBRARY_API bool FindNPCss(std::vector<int> npc, int maxdistance, int accuracy, int lifepoint, WPOINT tilespot, int maxdistance2, int action, std::string sidetext);

	//
	LIBRARY_API bool FindNPCssMulti(std::vector<int> npc, int maxdistance, int accuracy, int lifepoints, WPOINT tilespot, int maxdistance2, int action, std::vector<std::string> sidetext);

	//
	LIBRARY_API bool FindNPCssSTRRem(std::string NPC_name, int maxdistance, int accuracy, int lifepoints, WPOINT tilespot, int maxdistance2, int action, std::string sidetext);

	//
	LIBRARY_API bool FindNPCssSTR(std::string NPC_name, int maxdistance, int accuracy, int lifepoints, WPOINT tilespot, int maxdistance2, int action, std::string sidetext);

	//simple npc click
	LIBRARY_API bool FindNPCssSTR_FA(std::string NPC_name, int maxdistance, std::string sidetext);

	//
	LIBRARY_API bool FindNPCssSTRs(std::vector<std::string> NPC_names, int maxdistance, int accuracy, int lifepoints, WPOINT tilespot, int maxdistance2, int action, std::string sidetext);

	//
	LIBRARY_API int ReadPlayerAnim();

	//for loot list
	LIBRARY_API std::string GetItemText(int ID);

	//check if item is selected
	LIBRARY_API bool IsSelectingItem();

	//
	LIBRARY_API Choption ReadTargetInfo();

	//
	LIBRARY_API AllObject ReadLpInteracting();

	//
	LIBRARY_API std::vector<AllObject> OthersInteractingWithLpNPC(bool animated_also, int hp);

	//
	LIBRARY_API std::vector<AllObject> OthersInteractingWithLpPl(bool look_stance);

	//Get local player position in tile form
	LIBRARY_API FFPOINT PlayerCoordfloat();

	//norm tiles
	LIBRARY_API FFPOINT PlayerCoordfloat2();

	//Get local player position in raw form
	LIBRARY_API FFPOINT PlayerCoordfloatRaw();

	//
	LIBRARY_API void RandomSleep();

	//sleepy, First static wait time, second short common, third rarer long sleep
	LIBRARY_API void RandomSleep2(int wait, int sleep, int sleep2);

	//
	LIBRARY_API void TypeOnkeyboard(std::string asciii);

	//windoz keyboard typing
	LIBRARY_API void TypeOnkeyboard2(std::string asciii);

	//for localplayer: waits few cycles and then reports did it find any animation.
	LIBRARY_API bool CheckAnim(int Loops);

	//
	LIBRARY_API bool InvFull_();

	//
	LIBRARY_API int Invfreecount_();

	//
	LIBRARY_API bool ReadPlayerMovin();

	//
	LIBRARY_API bool ReadPlayerMovin2();

	//
	LIBRARY_API bool FindObj(std::vector<int> obj, int maxdistance, int accuracy, bool usemap, int action, std::string sidetext);

	//
	LIBRARY_API bool FindObj(std::vector<int> obj, int maxdistance, int accuracy, bool usemap, int action, std::vector<std::string> sidetext);

	//
	LIBRARY_API bool FindHl(std::vector<int> obj, int maxdistance, int accuracy, bool usemap, int action, std::string sidetext, std::vector<int> highlight);

	//
	LIBRARY_API bool FindObjCheck(std::vector<int> obj, int maxdistance, int accuracy, bool usemap, int action, std::string sidetext);

	//
	LIBRARY_API bool ClickAllObj(std::vector<AllObject> AllStuff2, int accuracy, bool usemap, int action, std::string sidetext);

	//
	LIBRARY_API bool ClickAllObj(std::vector<AllObject> AllStuff2, int accuracy, bool usemap, int action, std::vector<std::string> sidetext);

	//look for specific objects, return all data 
	LIBRARY_API std::vector<AllObject> GetAllObjArray(std::vector<int> obj, int maxdistance, vector< int> types);

	//look for specific objects, return all data, check if action text is there
	LIBRARY_API std::vector<AllObject> GetAllObjArrayInteract(std::vector<int> obj, int maxdistance, vector< int> types);

	//
	LIBRARY_API std::vector<AllObject> GetAllObjArrayInteract_str(std::vector<std::string> obj, int maxdistance, vector< int> types);

	//
	LIBRARY_API std::vector<AllObject> GetAllObjArray(std::vector<int> obj, int maxdistance, vector< int> types, WPOINT tile);

	//check tile for any disruptive object, returns false if nothing is found
	LIBRARY_API bool CheckTileforObjects(WPOINT tile);

	// Check specific tile for specific object, returns false if nothing is found
	//	Useful for checking for things like stumps while woodcutting
	LIBRARY_API bool CheckTileforObjects(WPOINT tile, int object, float thresh = 1.5);

	//check specific tile for specific item, returns false if nothing is found
	LIBRARY_API bool CheckTileforItems(WPOINT tile, int item);

	//
	LIBRARY_API bool FindObjTile(std::vector<int> obj, int maxdistance, int accuracy, WPOINT objtile, bool usemap, int action, std::string sidetext);

	//
	LIBRARY_API bool FindObjCheck_(std::vector<int> obj, int maxdistance, int accuracy, bool usemap, int action, std::string sidetext);

	//
	LIBRARY_API bool FindObjCheck_(std::vector<int> obj, int maxdistance, int accuracy, bool usemap, int action, std::string sidetext, WPOINT tile);

	//
	LIBRARY_API POINT MousePos_();

	//
	LIBRARY_API void MouseCLRS(POINT cursor, bool type);

	//
	LIBRARY_API void MouseMove_(POINT cursor);

	//
	LIBRARY_API void MouseMove_2(int x, int y, int rx, int ry);

	//
	LIBRARY_API float Hypot(float dx, float dy);

	//
	LIBRARY_API void MoveMouse2(int x, int y, int rx, int ry);

	//
	LIBRARY_API void MoveMouse3(int x, int y, int rx, int ry, bool updown);

	//
	LIBRARY_API void MouseLeftClick(int sleep, int rand);

	//
	LIBRARY_API void MouseRightClick(int sleep, int rand);

	//
	LIBRARY_API bool ClickInv_(int item, int randomelement, int action, int xrand = 0, int yrand = 0);

	//
	LIBRARY_API void KeyPress_(char mK);

	//
	LIBRARY_API void KeyPress_2(int mK);

	//
	LIBRARY_API void Send_MouseLeftClick(int sleep, int random);

	//
	LIBRARY_API void Send_MouseRightClick(int sleep, int random);

	//
	LIBRARY_API void Post_MouseMove(int x, int y);

	//
	LIBRARY_API void Post_MouseMoveAtoB(int x, int y);

	//
	LIBRARY_API void Post_MouseLeftClick(int x, int y, int sleep, int random);

	//
	LIBRARY_API void Post_MouseLeftClick_AtoB(int x, int y, int sleep, int random);

	//
	LIBRARY_API void Post_MouseRightClick(int x, int y, int sleep, int random);

	//
	LIBRARY_API int InvItemcount_(int item);

	//finds inv item
	LIBRARY_API bool InvItemFound(int item);

	//finds inv item
	LIBRARY_API bool InvItemFound(std::vector<int> items);

	//
	LIBRARY_API int InvItemcount_String(std::string item);

	//finds inv item stack size
	LIBRARY_API int InvItemcountStack_String(std::string item);

	//
	LIBRARY_API std::vector<int> InvItemcount_(std::vector<int> item);

	//
	LIBRARY_API uint64_t InvStackSize(int item);

	//
	LIBRARY_API uint64_t SystemTime();

	//
	LIBRARY_API int CRC32CheckSum(void* Data, int Size, int InitialValue);

	//
	LIBRARY_API std::string FindSideText();

	//
	LIBRARY_API bool SideTextEq(std::string text);

	//
	LIBRARY_API bool SideTextEq(std::vector<std::string> text);

	//active obj 0, npc 1, player 2, grounditems 3, pro 5, decor 12
	LIBRARY_API std::vector<AllObject> ReadAllObjectsArray(vector< int> types, vector< int> ids, vector< std::string> name_strings = {});

	//Reads inventory content and stores it in: InvArr
	LIBRARY_API std::vector<IInfo> ReadInvArrays33();

	//Reads cont
	LIBRARY_API std::vector<inv_Container> GetContainerSettings();

	//Reads, replacement for ReadInvArrays33
	LIBRARY_API std::vector<IInfo> Container_Inventory();

	//Get container vector data
	LIBRARY_API std::vector<WPOINT> Container_Get_all(int cont_id);

	//Get container single target
	LIBRARY_API WPOINT Container_Get_s(int cont_id, int item_id);

	//
	LIBRARY_API bool OpenEquipInterface2();

	//
	LIBRARY_API bool OpenInventoryInterface2();

	//
	LIBRARY_API void Get_shop();

	//
	LIBRARY_API int GetPray_();

	//
	LIBRARY_API int GetPrayMax_();

	//
	LIBRARY_API std::bitset<32> VB_ToBitSet(int value);

	//
	LIBRARY_API std::bitset<32> VB_ModifyBitSet(std::bitset<32> set, int at, bool state);

	//
	LIBRARY_API void VB_PrintBitsSet(std::bitset<32> to_print);

	//
	LIBRARY_API std::bitset<32> VB_Bitstr_convert(std::string str);

	//
	LIBRARY_API void VB_PrintBits_all(int to_print);

	//
	LIBRARY_API void VB_PrintBits(int ID);

	//
	LIBRARY_API std::bitset<32> VB_GetBits(int ID);

	//
	LIBRARY_API int VB_GetBit(int ID, int at, int FSarray = -1);

	//read varpbit directly
	LIBRARY_API VB VB_ReadBits(VB bitaddr);

	//
	LIBRARY_API bool GetInCombBit();

	//
	LIBRARY_API bool IsTargeting();

	//
	LIBRARY_API int GetAddreline_();

	//
	LIBRARY_API bool Compare2874Status(int status, bool debug = false);

	//
	LIBRARY_API bool CheckBankVarp();

	//
	LIBRARY_API int GetG3095Status();

	//
	LIBRARY_API int GetHP_();

	//
	LIBRARY_API int GetHPMax_();

	//%
	LIBRARY_API int GetHPrecent();

	//Pray points left in %
	LIBRARY_API int GetPrayPrecent();

	//
	LIBRARY_API int GetSummoningPoints_();

	//
	LIBRARY_API int GetSummoningMax_();

	//
	LIBRARY_API void PIdle1();

	//
	LIBRARY_API void PIdle22();

	//
	LIBRARY_API void PIdle2();

	//item in area//xc is expected pos//xcurrent is actual//
	LIBRARY_API bool InArea(int xc, int rangex, int xcurrent, int yc, int rangey, int ycurrent);

	//item in area//xc is expected pos//xcurrent is actual//
	LIBRARY_API bool InArea(float xc, float rangex, float xcurrent, float yc, float rangey, float ycurrent);

	//
	LIBRARY_API bool InArea2(int xstart, int xend, int xcurrent, int ystart, int yend, int ycurrent);

	//
	LIBRARY_API bool InArea2(float xstart, float xend, float xcurrent, float ystart, float yend, float ycurrent);

	//check x to xrange..,floorz are not checked
	LIBRARY_API bool PInArea(int x, int xrange, int y, int yrange, int zfloor = 0);

	//normal tile, range in tiles, straight
	LIBRARY_API bool PInAreaF(FFPOINT norm_tile, float range);

	//normal tile, range in tiles, rounded
	LIBRARY_API bool PInAreaF(FFPOINT norm_tile, int range);

	//normal tile, range in tiles
	LIBRARY_API bool PInAreaW(WPOINT norm_tile, int range);

	//
	LIBRARY_API bool PInArea2(int xstart, int xend, int ystart, int yend);

	//
	LIBRARY_API bool PInArea2(float xstart, float xend, float ystart, float yend);

	//
	LIBRARY_API void KeyboardPress(char codes, int sleep, int rand);

	//
	LIBRARY_API void KeyboardPress2(int codes, int sleep, int rand);

	//use keyboard
	LIBRARY_API void KeyboardPress3(unsigned char codes, int sleep, int rand);

	//use keyboard, keymod is shift or alt
	LIBRARY_API void KeyboardPress3(unsigned char codes, int keymod = 0);

	//
	LIBRARY_API void ccout(int color, std::string tex);

	//
	LIBRARY_API bool InvRandom_(int action);

	//Check that inventory is open
	LIBRARY_API bool InvCheck1_();

	//Play windows alarm, sound will come trough client so turn rs sound up on windows mixer
	LIBRARY_API void Play_sound(int sleeptime = 100, std::string location = "C:\\Windows\\Media\\ringout.wav");

	//check every possible location/data_base - data block start
	/*
	data_base - IInfo - pass original to update original
	refresh_pixels - interface box location and size update
	refresh_data - update item ids
	refresh_text - attempt to read text
	*/
	LIBRARY_API IInfo GetInterfaceDataIInfo(IInfo* database, bool refresh_data, bool refresh_text);

	/*
	data_base - info block start
	refresh_pixels - interface box location and size update
	refresh_data - update item ids
	refresh_text - attempt to read text
	*/
	LIBRARY_API IInfo GetInterfaceDataMem(uint64_t database, bool refresh_pixels, bool refresh_text);

	//find, check, get data, for IInfo
	//refresh_data - xy and item int data
	//refresh_text - item text
	LIBRARY_API IInfo Get_Check_Interface(IInfo* current_data, std::vector<InterfaceComp5> search_components, bool refresh_data = false, bool refresh_text = false);

	//find, check, for IInfo, keep tabs on 1 interface and return interfaces under it
	LIBRARY_API std::vector<IInfo> Get_Check_Interface_Under(IInfo* current_data, std::vector<InterfaceComp5> search_components);

	LIBRARY_API IInfo Get_Check_Interfacer(IInfo* current_data, std::vector<InterfaceComp5> search_components, bool refresh_data = false, bool refresh_text = false);

	//
	LIBRARY_API FFPOINT ToMapFFPOINT(FFPOINT ItemCoord, bool map_limit = false);

	//
	LIBRARY_API bool ClickMapTile_(FFPOINT ItemCoord2);

	//
	LIBRARY_API bool ClickMapTile_2(POINT ItemCoord2);

	/*
	finds and reads varpbits
	as there can be 2 vars lets try different filters to find what we need from 0-2
	*/
	LIBRARY_API VB VB_FindPSett(int id, int try_filter = 0);

	//
	LIBRARY_API std::vector<bool> VB_FindPSett2(int id, int try_filter = 0);

	//makes ints into bit arrays for displaying
	LIBRARY_API std::string VB_IntToBit(int var);

	//
	LIBRARY_API int VB_FindPSett3int(int id, int spot_index, int try_filter = 0);

	//
	LIBRARY_API WPOINT VB_FindPSett3wpoint(int id, int spot_index1, int spot_index2, int try_filter = 0);


	//check if variable is between
	LIBRARY_API bool Math_VarBetween(int start, int end, int checked_var);

	LIBRARY_API std::vector<IInfo> MaterialStorage();

	//
	LIBRARY_API std::vector<IInfo> TradeWindow(std::string which);

	//
	LIBRARY_API bool LootWindowOpen_2();

	//get LootWindow data, LootWArr array
	LIBRARY_API std::vector<IInfo> LootWindow_GetData();

	//
	LIBRARY_API int LootWindow_space_needed(std::vector<int> Except_item, bool Inventory_stacks);

	//
	LIBRARY_API bool LootWindow_Loot(std::vector<int> Except_itemv);

	//
	LIBRARY_API bool SelectCOption_(std::string choice);

	//
	LIBRARY_API WPOINT SelectCOption2(std::string choice, std::string user);

	//
	LIBRARY_API bool SelectCOption2_(std::string choice, std::string user);

	//
	LIBRARY_API WPOINT SelectCOption(std::string choice);

	//
	LIBRARY_API bool SelectCOption_Click(std::string choice, bool move);

	//string filter
	LIBRARY_API std::string Filter(std::string to, std::string remove);

	//add space
	LIBRARY_API std::string Filter2(std::string to, std::string remove);

	//string filter ,copy everything after keyword
	LIBRARY_API std::string String_Filter(std::string sentence, std::string keyword);

	//string filter, remove that color info
	LIBRARY_API std::string String_Filter2(std::string to);

	//string filter, remove that color info, without space
	LIBRARY_API std::string String_Filter22(std::string to);

	//string filter, remove that color info, specialy made for getting player name
	LIBRARY_API std::string String_Filter3(std::string to);

	//
	LIBRARY_API bool FindChooseOptionOpen();

	//
	LIBRARY_API bool FindChooseOptionOpenClose();

	//
	LIBRARY_API bool Math_Compare_AllObject_dist_smallest(AllObject SummAddress1, AllObject SummAddress2);

	////sorting function, clue
	LIBRARY_API bool Math_Compare_clue_biggest(clue_arrow SummAddress1, clue_arrow SummAddress2);

	//sorting function, find smallest
	LIBRARY_API bool Math_Compare_smallest(int Summ1, int Summ2);

	//sorting function, find smallest
	LIBRARY_API bool Math_Compare_smallest_FF(FFPOINT Summ1, FFPOINT Summ2);

	//sorting function, find biggest
	LIBRARY_API bool Math_Compare_biggest(int Summ1, int Summ2);

	//sorting function, by id1
	LIBRARY_API bool Math_Compare_InterfaceComp5_smallest(InterfaceComp5 SummAddress1, InterfaceComp5 SummAddress2);

	//find 1 int id from Bbar ids array
	LIBRARY_API bool Math_Bbar_ValueEquals(int value, const std::vector<Bbar>& arrayof);

	//check multiplies ints vs Bbar ids, returns answers same order as put in
	LIBRARY_API std::vector<bool> Math_Bbar_ValueEqualsArr(const std::vector<int>& arrayof1, const std::vector<Bbar>& arrayof2);

	//find 1 int id from AllObject ids array
	LIBRARY_API bool Math_AO_ValueEquals(int value, const std::vector<AllObject>& arrayof);

	//find 1 int id from IInfo ids array
	LIBRARY_API bool Math_IInfo_ValueEquals(int value, const std::vector<IInfo>& arrayof);

	//Go trough all ids vs IInfo ids array
	LIBRARY_API bool Math_IInfo_ValueEquals(const std::vector<int>& value, const std::vector<IInfo>& arrayof);

	//Name vs IInfo names array
	LIBRARY_API bool Math_IInfo_ValueEqualsStr(std::string_view name, const std::vector<IInfo>& arrayof);

	//Vector of Names vs IInfo names array
	LIBRARY_API bool Math_IInfo_ValueEqualsStrArr(const std::vector<std::string>& name, const std::vector<IInfo>& arrayof);

	//find 1 int id from IInfo ids array, check if stack
	LIBRARY_API bool Math_IInfo_ValueEqualsStack(int value, const std::vector<IInfo>& arrayof);

	//find array of int ids to match AllObject ids match
	LIBRARY_API bool Math_AO_ValueEqualsArr(const std::vector<int>& arrayof1, const std::vector<AllObject>& arrayof2);

	//check multiplies ints vs AO ids, returns answers same order as put in
	LIBRARY_API std::vector<bool> Math_AO_ValueEqualsArr2(const std::vector<int>& arrayof1, const std::vector<AllObject>& arrayof2);

	//random number generator thing
	LIBRARY_API double Math_RandomNumber(double number);

	//random number generator thing
	LIBRARY_API float Math_RandomNumber(float number);

	//random number generator thing
	LIBRARY_API int Math_RandomNumber(int number);

	//reverse int
	LIBRARY_API int Math_Reverse_int(int val);

	//glue bytes together, into 16bit
	LIBRARY_API unsigned short Math_AppendBytes16(unsigned char* val, int start);

	//glue bytes together, into 16bit
	LIBRARY_API unsigned short Math_AppendBytes16rev(unsigned char* val, int start);

	//glue bytes together, into 24bit, return standard int
	LIBRARY_API int Math_AppendBytes24(unsigned char* val, int start);

	//glue bytes together in reverse, into 24bit, return standard int
	LIBRARY_API int Math_AppendBytes24rev(unsigned char* val, int start);

	//glue bytes together, into 32bit
	LIBRARY_API int Math_AppendBytes32(unsigned char* val, int start);

	//glue bytes together, into 64bit
	LIBRARY_API uint64_t Math_AppendBytes64(unsigned char* val, int start);

	//glue bytes together, into 64bit
	LIBRARY_API uint64_t Math_AppendBytes64rev(unsigned char* val, int start);

	//chop into bytes
	LIBRARY_API std::vector<uint8_t> Math_Chopbytes16(unsigned short val);

	//chop into bytes
	LIBRARY_API std::vector<uint8_t> Math_Chopbytes32(int val);

	//chop into bytes
	LIBRARY_API std::vector<uint8_t> Math_Chopbytes64(uint64_t val);

	//compare vector of ints vs one integer
	LIBRARY_API bool Math_Compare_int(const std::vector<int>& inputaddresses, int target);

	//
	LIBRARY_API bool Math_Compare_AllObject_dist_biggest(AllObject SummAddress1, AllObject SummAddress2);

	//for debubg
	LIBRARY_API void FindChooseOptionLive();

	//find it
	LIBRARY_API void FindChooseOption();

	//dno
	LIBRARY_API bool SelectCOption_Direct(int index);

	//
	LIBRARY_API bool ReadNPCInFocus_0();

	//
	LIBRARY_API bool ReadNPCInFocus(int index, bool debug);

	//
	LIBRARY_API bool GetCheckNPCInFocus(int index, int NPC_id, bool debug);

	//read allobj, 3d screen to 2d pixels, Client sees
	LIBRARY_API WPOINT W2ScreenNat(uint64_t spot);

	//3d coords to screen. Client
	LIBRARY_API WPOINT W2ScreenNat(FFPOINT entity);

	//3d coords to screen. returns float point
	LIBRARY_API FFPOINT W2ScreenNat_F(FFPOINT entity);

	//npc uptext watch
	LIBRARY_API bool WatchText3(std::string text, int action, int loops);

	//
	LIBRARY_API std::vector<std::string> ListPlayers_();

	//
	LIBRARY_API bool FindPlayerbyName2_(std::string name, std::string option);

	//
	LIBRARY_API bool FindPlayerbyNames_(std::vector<std::string> name, std::string option, bool usename);

	//
	LIBRARY_API bool FindPlayerbyNames_(std::vector<std::string> name, std::string option, bool usename, bool checkcombat, int xstart, int xend, int ystart, int yend);

	//
	LIBRARY_API bool FindRandomPlayers_(std::string option, bool usename, bool checkcombat, int xstart, int xend, int ystart, int yend);

	// Return true if any player not on whitelist is in the range
	LIBRARY_API bool RandomPlayersInRange(std::vector<std::string> whitelist_names, int distance);

	//
	LIBRARY_API bool FindRandomPlayers_(std::string option, bool usename);

	//find other player vs local, return closest
	LIBRARY_API AllObject OtherPlayerIsInteractingWithVSLocal();

	//That hovering bar that apears near player when progress is happening
	LIBRARY_API int LocalPlayer_HoverProgress();

	//is player interacting?  Any "player" vs interacting
	LIBRARY_API bool PlayerInterActing_(std::string player);

	//Any "player"
	LIBRARY_API bool IsInCombat_(std::string player);

	//try to deter if player is in combat by stance id, localplayer for speed
	LIBRARY_API bool LocalPlayer_IsInCombat_();

	//Local player with who is player interacting?. Gets the name. Speed
	LIBRARY_API std::string Local_PlayerInterActingWith_();

	//Local player with who is player interacting?. Gets the ID. Speed
	LIBRARY_API int Local_PlayerInterActingWith_Id();

	//Local player is interacting? retuns true if anything is found
	LIBRARY_API bool Local_PlayerInterActingWith_2(int loop);

	//Local player is interacting? vs name, retuns true if is found
	LIBRARY_API bool Local_PlayerInterActingWith_2(int loop, std::string name);

	//Any "player"
	LIBRARY_API int GetPlayerAnimation_(std::string player);

	//Any "player"
	LIBRARY_API bool IsPlayerAnimating_(std::string player, int loops);

	//Any "player"
	LIBRARY_API bool IsPlayerMoving_(std::string player);

	//Compare. Any "player" vs interacting
	LIBRARY_API bool PlayerInterActingWithCompare_(std::string player1, std::string entity);

	//with who is player interacting?  Any "player" vs interacting
	LIBRARY_API std::string PlayerInterActingWith_(std::string player);

	//with who is player interacting? Read directly at location.
	LIBRARY_API std::string PlayerInterActingWith_(uint64_t localmem);

	//
	LIBRARY_API std::string Local_PlayerInterActingWith_str(int loop);

	//
	LIBRARY_API int Local_PlayerInterActingWith_id(int loop);

	// Get interface by whole chain of id steps, accurate and more faster
	LIBRARY_API IInfo GetInterfaceByIds(std::vector<InterfaceComp5> target, bool I_text = false);

	//Get interface by whole chain of id steps, accurate and more faster, gather interfaces (like invboxes) info under this interface
	LIBRARY_API std::vector<IInfo> GetInterfaceByIdsUnder(std::vector<InterfaceComp5> target, int offset = 0, bool I_text = false);

	//
	LIBRARY_API std::vector<IInfo> ScanForInterfaceTest2Get(bool target_under, std::vector<InterfaceComp5> lv_ID);

	//gets game status. if in game 3, if lobby 2, if logged out 1, unknown 0, loading 4
	LIBRARY_API int GetGameState2();

	//
	LIBRARY_API int GetAngle();

	//
	LIBRARY_API int GetTilt();

	//
	LIBRARY_API bool GetQuickPray();


	LIBRARY_API int GetTargetHealth();

	// Using 463
	LIBRARY_API bool GetRun();

	// Using 119
	LIBRARY_API bool GetRun2();

	//rounded down and int
	LIBRARY_API WPOINT PlayerCoord();

	//outputs array of points from a to b
	LIBRARY_API std::vector<WPOINT> Math_Bresenham_line(int x1, int y1, int x2, int y2);

	//outputs array of points from a to b
	LIBRARY_API std::vector<WPOINT> Math_Bresenham_lineW(WPOINT xy1, WPOINT xy2);

	//outputs array of points from a to b
	LIBRARY_API std::vector<WPOINT> Math_Bresenham_lineF(FFPOINT xy1, FFPOINT xy2);

	//input raw tiles to destination, output pixels to minimap
	//take 1 click foward to destination
	LIBRARY_API WPOINT Bresenham_step(FFPOINT tilexy);

	//Highlight mark up tile. highlight is that sparkle id /mining 7164, arch 7307, localp_dist = howfar player could go/be
	LIBRARY_API bool FindHLvsLocalPlayer(std::vector<int> objIds, int maxdistance, int accuracy, bool usemap, int action, std::string sidetext, std::vector<int> hlIds, float localp_dist = 2.f);

	//
	LIBRARY_API bool FindObjRot(std::vector<int> obj, int maxdistance, float sens);

	//
	LIBRARY_API bool RotateCamera(FFPOINT ItemXY, FFPOINT currxy, float sens);

	//
	LIBRARY_API FFPOINT FindNPCbyName(std::string name);

	//
	LIBRARY_API AllObject FindNPCbyName2(std::string name, int distance);

	//
	LIBRARY_API bool FindHObj(std::vector<int> obj, int maxdistance, int accuracy, bool usemap, int action, std::string sidetext);

	//
	LIBRARY_API bool ClickInv_(std::string item, int randomelement, int action, int xrand = 0, int yrand = 0);

	//
	LIBRARY_API bool ClickInv_Multi(std::vector<int> items, int randomelement, int action);

	//
	LIBRARY_API bool ClickInvOffset_(int item, int action, int randx, int randy, int offsetx, int offsety, std::string sidetext);

	//
	LIBRARY_API FFPOINT FindObjTileName(std::vector<int> obj, int maxdistance);

	//
	LIBRARY_API void ReadChatMessages();

	//
	LIBRARY_API void ReadChatMessages_G();

	//
	LIBRARY_API void ReadChatMessages_Save(std::string file_name);

	//
	LIBRARY_API std::vector<std::string> GetChatMessage(int Line_index, int size);

	//
	LIBRARY_API std::vector<std::string> ChatMessageCompareExtra(std::string text, std::string text1_2, int size);

	//
	LIBRARY_API std::vector<std::string> ChatMessageCompare(std::string text, int size);

	//
	LIBRARY_API std::vector<std::string> ChatMessageCompare(std::string textcolor, std::string text, int size);

	//
	LIBRARY_API std::vector<std::string> ChatMessageCompare(std::string playername, std::string textcolor, std::string text, int size);
	//
	LIBRARY_API void Map_Walker1(FFPOINT tilexy, int distance);

	//
	LIBRARY_API void Map_Walker1NT(WPOINT tilexy2, int distance);

	//
	LIBRARY_API void Map_Walker2(FFPOINT tilexy, int distance);
};

namespace MEX {

	//Looks if localplayer is member
	LIBRARY_API bool IsMember();

	//choose teleport menu option that comes up with jewelry, uses keys, it is case sensetive
	LIBRARY_API bool Teleport_Option(std::string text);

	// Get the current tick number.
	LIBRARY_API uint32_t Get_tick();

	// Sleeps until a specified amount of ticks have passed.
	LIBRARY_API void Sleep_tick(int count);

	/// check if gametick has changed since last asked with using the same function
	LIBRARY_API bool Check_tick();

	// Initialize the counter.
	LIBRARY_API void Tick_counter_init();

	// Checks if the given number of ticks have passed since Tick_counter_init() was called last.
	LIBRARY_API bool Ticks_passed(uint64_t count);

	//count them
	LIBRARY_API bool Count_ticks(int count);

	//
	LIBRARY_API bool isAbilityAvailable(const std::string& ability_name);

	//
	LIBRARY_API float calculatePlayerOrientation();

	//any entitity, needs Allobject MemE
	LIBRARY_API float calculateOrientation(uint64_t mem_addr);

	//
	LIBRARY_API void RandomEvents();

	//Get familiar name
	LIBRARY_API std::string GetFamiliarName();

	//check if familiar is present
	LIBRARY_API bool CheckFamiliar();

	//
	LIBRARY_API std::vector<uint64_t> GetModel_ids(uint64_t entity_base, bool debug);

	//Compare model ids
	LIBRARY_API bool ModelCompare(uint64_t entity_base, std::vector<uint64_t> model_ids);

	//Look from players. Compare model ids
	LIBRARY_API bool FindModelCompare(std::string name, std::vector<uint64_t> model_ids);

	//Read chat into vectors
	LIBRARY_API std::vector<ChatTexts> ChatGetMessages();

	//find latest message, any part of it will do
	LIBRARY_API ChatTexts ChatFind(std::string text, int limit);

	//to int
	LIBRARY_API int AsciiToNumbers32(std::string ascii_num);

	//to uint64_t
	LIBRARY_API uint64_t AsciiToNumbers64(std::string ascii_num);

	//vector of strings to vector of int numbers
	LIBRARY_API std::vector<int> StringsToInts(std::vector<std::string> input);

	//special for getting portable time from chat
	LIBRARY_API int ChatPortableTime();

	//tries to read, equipment must be open. Maybe make some other function for checking that 
	LIBRARY_API std::vector<IInfo> ReadEquipment();

	//get slot
	LIBRARY_API IInfo GetEquipSlot(int slot);

	//print slots
	LIBRARY_API void PrintEquipSlots();

	//slot equals id
	//0 hat,1 cape,2 ammy,3 weapon,4 body,5 off-hand,6 legs,7 gloves,8 boots,9 ring,10 arrows, 11 aura, 12 scrim/book
	LIBRARY_API bool EquipSlotEq(int slot, int id);

	//slot equals string
	//0 hat,1 cape,2 ammy,3 weapon,4 body,5 off-hand,6 legs,7 gloves,8 boots,9 ring,10 arrows, 11 aura, 12 scrim/book
	LIBRARY_API bool EquipSlotEq(int slot, std::string txt);

	//check if equipment is open, there is no point reading if it is not open
	LIBRARY_API bool EquipInterfaceCheckvarbit();

	//check if equipment is open, there is no point reading if it is not open
	LIBRARY_API bool InventoryInterfaceCheckvarbit();

	//check if Lodestone map is open, it probl some bit
	LIBRARY_API bool LODEInterfaceCheckvarbit();

	//check if Deposit interface is open, it probl some bit
	LIBRARY_API bool DEPOInterfaceCheckvarbit();

	//
	LIBRARY_API bool Find_NAMEdata_match(std::vector<NameData> in, std::string compare_against);

	//
	LIBRARY_API AllObject GetAllObj_dist(int obj, int distance, int type, WPOINT adjust_tile);

	LIBRARY_API std::unordered_map<int, int> GetExchangePrice(const std::vector<int> itemIds);

	//
	LIBRARY_API int GetExchangePrice(int itemid);

	//
	LIBRARY_API bool BankGetVisItemsPrint();

	//
	LIBRARY_API WPOINT BankGetItem(int item);

	//
	LIBRARY_API bool BankClickItem(int id, int mouse);

	//
	LIBRARY_API int BankGetItemStack(int item);
	//
	LIBRARY_API int BankGetItemStack(std::string itemname);
	// Finds objects by name
	LIBRARY_API std::vector<AllObject> FindObject_str(std::vector<std::string> obj, int maxdistance);
	//
	LIBRARY_API std::vector<int> BankGetItemStack(std::vector<int> item);

	//for buffbar, convert all time to seconds
	LIBRARY_API int Bbar_ConvToSeconds(Bbar bar);

	//
	LIBRARY_API std::vector<Bbar> Buffbar_GetAllIDs(bool print_all_out);

	//
	LIBRARY_API Bbar Buffbar_GetIDstatus(int id, bool debug);

	//
	LIBRARY_API std::vector<Bbar> DeBuffbar_GetAllIDs(bool print_all_out);

	//check debuff status based id
	LIBRARY_API Bbar DeBuffbar_GetIDstatus(int id, bool debug);

	//main ability bar, get all critical data
	LIBRARY_API std::vector<Abilitybar> GetABarInfo(int bar_nr);

	//print specified ability(container) bar 0-2
	LIBRARY_API void print_GetABarInfo(int bar_nr);

	//debug, print all 0-2
	LIBRARY_API void GetABarInfo_DEBUG();

	// get ability action texts
	//std::vector<string> GetABarActions(int bar_nr);

	//get ability based on name
	LIBRARY_API Abilitybar GetAB_name(int bar_nr, std::string ability_name);

	//get ability based on id
	LIBRARY_API Abilitybar GetAB_id(int bar_nr, int ability_id);

	//get ability based on name, check 3 first bars, returns first found
	LIBRARY_API Abilitybar GetABs_name(std::string ability_name);

	//get ability based on name, check 3 first bars, returns first found, returns same order as in
	LIBRARY_API std::vector<Abilitybar> GetABs_name(std::vector<std::string> ability_name);

	//
	LIBRARY_API WPOINT BankGetLimits();

	//
	LIBRARY_API bool BankAllItems();

	//
	LIBRARY_API bool BankAllItem_InvExceptintM(std::vector<int> Except_item);

	//
	LIBRARY_API bool BankAllItem_InvExceptstrM(std::vector<std::string> Except_item);

	//
	LIBRARY_API WPOINT BankGetItem_Inv(int item);

	//
	LIBRARY_API bool BankClickItem_Inv(int id, int mouse);

	//
	LIBRARY_API bool BankClickItem_InvChoose(int id, std::string choose_text);

	//
	LIBRARY_API int BankGetItemStack_Inv(int item);

	//
	LIBRARY_API bool Wait_Timer(int wait_time, int random_time, bool reset);

	LIBRARY_API bool isProcessing();

	//
	LIBRARY_API ReturnText ScriptDialogWindow2(std::string boxtext, std::vector<std::string> textchoices, std::string button_name1 = "Start", std::string button_name2 = "Close", std::string Make = "", std::string Edit = "");

	//returns selection, always returns 0,1 slot
	LIBRARY_API std::vector <std::string> ScriptAskBox(std::string boxtext, bool secondedit);

	//
	LIBRARY_API std::vector <NameData> ScriptDialogWindow_input(std::string boxtext, bool password, int arrtype, std::string filename);

	//kinda extended vbs for some reason, get value
	LIBRARY_API int GetExtendedVB(VB set);

	////xp vs level
	LIBRARY_API int XPLevelTable(int xp, bool elite = false);

	/// level to xp
	LIBRARY_API int XPForLevel(int level, bool elite = false);

	//Get current xp
	/*
	ATTACK
	STRENGTH
	RANGED
	MAGIC
	DEFENCE
	CONSTITUTION
	PRAYER
	SUMMONING
	DUNGEONEERING
	AGILITY
	THIEVING
	SLAYER
	HUNTER
	SMITHING
	CRAFTING
	FLETCHING
	HERBLORE
	RUNECRAFTING
	COOKING
	CONSTRUCTION
	FIREMAKING
	WOODCUTTING
	FARMING
	FISHING
	MINING
	DIVINATION
	INVENTION not known
	ARCHAEOLOGY
	*/
	LIBRARY_API int GetSkillXP(std::string name);


	LIBRARY_API bool IsSkillsPanelOpen();

	LIBRARY_API bool ToggleSkillsPanelVisibility();

	// id starts at 0, goes left to right, top to bottom. Attack is 0 smithing is 5
	LIBRARY_API int GetBoostedSkillLevel(int id, int currentLevel);

	LIBRARY_API int GetBoostedSkillLevel(int id);

	// id starts at 0, goes left to right, top to bottom. Attack is 0 smithing is 5
	LIBRARY_API Skill GetSkillById(int id);

	LIBRARY_API Skill GetSkillByName(std::string name);
	//
	LIBRARY_API bool CheckDoItemOpen();

	//
	LIBRARY_API bool CheckDoToolOpen();

	//
	LIBRARY_API bool SelectToolOpen(std::string txt_to_find);

	//
	LIBRARY_API bool BankOpen2();

	//Get inventory status. Checks if items are present inventory
	LIBRARY_API bool CheckInvStuff(int item1, int item2);

	//Get inventory status. Checks if items are present inventory
	LIBRARY_API bool CheckInvStuff(int item1);

	//Get inventory status. Checks if items are present inventory
	std::vector<bool> CheckInvStuff(std::vector<int> item1);

	//Get inventory status. Checks if items are present inventory, check if all that
	LIBRARY_API bool CheckInvStuffCheckAll(std::vector<int> items);

	//Get inventory status. Checks if items are present inventory, size is amount to be considered ok
	LIBRARY_API std::vector<bool> CheckInvStuff(std::vector<int> items, int size);

	//Get inventory status. Checks if items are present inventory, size is amount to be considered ok
	LIBRARY_API bool CheckInvStuffCheckAll(std::vector<int> items, int size);

	//
	LIBRARY_API bool OpenBankChest(int chest);

	//open bank, push number
	LIBRARY_API bool OpenBankChest(int chest, char pushnumber);

	//open bank, check content, push number
	LIBRARY_API bool OpenBankChest(int chest, char pushnumber, std::vector<int> content_ids);

	//open bank, check content, push number(or dont if any them are 0), return found amounts, amount left is size
	LIBRARY_API std::vector<int> OpenBankChest_am(int chest, char pushnumber, std::vector<int> content_ids, int size);

	//
	LIBRARY_API bool OpenBankChest(int chest, int distance, std::string option, std::string sidetext);

	//
	LIBRARY_API bool WaitUntilMovingEnds();

	// waits until player is done
	LIBRARY_API bool WaitUntilMovingandAnimEnds();

	//
	LIBRARY_API bool DoPortables(int port, std::string checktext);

	//
	LIBRARY_API bool Notestuff(int item);

	LIBRARY_API bool DoNoteStuff(int item);

	//
	LIBRARY_API bool NotestuffInvfull(int item);

	//
	LIBRARY_API bool WalkUntilClose(WPOINT walktile, int stopdistance);

	//-1 dont check id, 1 check id is bigger than 0, or straight up 1170 varbit id
	LIBRARY_API bool DoPortables(int port, int settID, std::string checktext);

	//
	LIBRARY_API bool Smithing_interface_open();

	//
	LIBRARY_API void BankClose();
};

namespace IG {

	//removes all, should not to be used another thread
	LIBRARY_API void ImGui_clear_all(bool remove_perm = false);

	//Push onto que, Draw simple circle at static pixels
	LIBRARY_API void DrawCircle(bool permanent, IG_answer* return_);

	//Draw simple Square at static pixels
	LIBRARY_API void DrawSquare(bool permanent, IG_answer* return_);

	//Draw simple Square but calculate from normalized tile to pixels
	LIBRARY_API void DrawSquareNormTile(bool permanent, IG_answer* return_);

	//Push onto que, Draw simple circle but calculate from normalized tile to pixels, to make it more dynamic
	LIBRARY_API void DrawCircleNormTile(bool permanent, IG_answer* return_);

	//Push onto que, Draw simple circle but calculate from raw tile to pixels, to make it more dynamic
	LIBRARY_API void DrawCircleRawTile(bool permanent, IG_answer* return_);

	//track entity, needs local memory spot
	LIBRARY_API void DrawCircleEntity(bool permanent, IG_answer* return_);

	//track, normal tile with timeout
	LIBRARY_API void DrawCircleTimeOut(bool permanent, IG_answer* return_);

	//Push onto que, Draw Text at static pixels
	LIBRARY_API void DrawTextAt(bool permanent, IG_answer* return_);

	//Maybe save some settings, 20 slots
	LIBRARY_API bool SaveIntSetting(std::string foldername, std::string name, BYTE slot, BYTE val);

	//Maybe load some settings, 20 slots
	LIBRARY_API int LoadIntSetting(std::string foldername, std::string name, BYTE slot);

	//add drop down list
	LIBRARY_API void DrawComboBox(bool permanent, IG_answer* return_);

	//add list
	LIBRARY_API void DrawListBox(bool permanent, IG_answer* return_);

	//add square,intended for backdrops
	LIBRARY_API void DrawSquareFilled(bool permanent, IG_answer* return_);

	//push box1, ask_box1 = false; turn manually off again
	LIBRARY_API void DrawBox(bool permanent, IG_answer* return_);

	//push text1, turn manually off again
	LIBRARY_API void DrawTextInput(bool permanent, IG_answer* return_);

	//progress bar - uses radius as progress amount
	LIBRARY_API void DrawProgressBar(bool permanent, IG_answer* return_);

	//tick 1, box_bool returns state of tickbox, return_bool value is only needed to check if state changed
	LIBRARY_API void DrawCheckbox(bool permanent, IG_answer* return_);
};