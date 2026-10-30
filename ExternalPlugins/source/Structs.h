#pragma once
#include <cstdint>
#include <string>
#include <variant>
#include <vector>
#include <functional>
#include <ctime>
#include <imgui.h>
#include <sstream>
#include <optional>
#include <utility>

// Opaque RuneLite JNI handles. The plugin SDK does not make JNI calls, but
// these pointer-sized fields are part of the host's AllObject layout.
struct _jobject;
struct _jclass;
using jobject = _jobject*;
using jclass = _jclass*;

// Define a variant type for items, which can be either an int (item ID) or a string (item name)
using ItemType = std::variant<int, std::string>;

// RGBA pixel data fetched from RuneLite's SpriteManager
struct SpriteData {
	int width = 0;
	int height = 0;
	std::vector<uint8_t> rgba; // width * height * 4 bytes (R,G,B,A)
	bool valid = false;
};


//we store data

struct SPLAT {
	int Type = 0;
	int Amount = 0;
	int EInfo1 = 0;
	int EInfo2 = 0;
	uint64_t Time = 0;
	int Slot = 0;
	uint64_t OwnerAddr = 0;   //entity that owns the hitmark manager (splat target)
	int OwnerType = -1;       //entity type at OwnerAddr+OFF_OBJALL.allofftype (1=NPC, 2=player)
	std::string OwnerName{};  //eastl name at OwnerAddr+OFF_NPC.npcoff_NameEANPC

	SPLAT() = default;
	SPLAT(int _Type, int _Amount, int _EInfo1, int _EInfo2, uint64_t _Time, int _Slot,
		uint64_t _OwnerAddr = 0, int _OwnerType = -1, std::string _OwnerName = {}) :
		Type{ _Type }, Amount{ _Amount }, EInfo1{ _EInfo1 }, EInfo2{ _EInfo2 }, Time{ _Time }, Slot{ _Slot },
		OwnerAddr{ _OwnerAddr }, OwnerType{ _OwnerType }, OwnerName{ std::move(_OwnerName) } {}
};

//debug box strings
struct debugbox {
	std::string text{};//text
	int warn_level = 0;
};

//Bytes array, reserve with static_cast<unsigned char*>(calloc(1, static_cast<size_t>(1024))); need to be freed with free();
struct BytesContainer {
	unsigned char* bytes = nullptr;
	size_t size = 0;
};

//
struct debugboxPac {
	std::stringstream text{};//text
	BytesContainer bytes{};//raw bytes
	int warn_level = 0;
	int length = 0;//how many bytes
	std::string Name{};//descriptions
	unsigned char OP = 0;//id
	std::string MagicalBytes{};//signature numbers?
	std::vector < std::string> DataSections{};//Use words to indicate what useful data would be there, then implent it, 2 bytes generally. In order
};

// ground items info
struct GroundItems {
	int Id = 0;
	std::string Name{};
	bool noted = false;
};

// text choices
struct ReturnText {
	std::string Name{};
	int Nr = 0;
	int listNr = 0;

	ReturnText() = default;
	ReturnText(std::string _Name, int _Nr, int _listNr) : Name{ _Name }, Nr{ _Nr }, listNr{ _listNr } {}
};

// player name info
struct NameData {
	std::string Name{};
	std::string Password{};
	std::string Pin{};
};

// for coordinates
struct FFPOINT {
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	FFPOINT() = default;
	FFPOINT(float _x, float _y, float _z) : x{_x}, y{_y}, z{_z} {}

	FFPOINT(const ImVec2& f) : x(f.x), y(f.y) {}
	operator ImVec2() const { return ImVec2(x, y); }
};

// for coordinates, int size point
struct WPOINT {
	int x = 0;
	int y = 0;
	int z = 0;

	WPOINT() = default;
	WPOINT(int _x, int _y, int _z) : x{_x}, y{_y}, z{_z} {}

	static WPOINT FromLocal(int localX, int localY, int plane);

	WPOINT(const FFPOINT& f) : x(static_cast<int>(f.x)), y(static_cast<int>(f.y)), z(static_cast<int>(f.z)) {}
	operator FFPOINT() const { return FFPOINT(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)); }
	WPOINT(const ImVec2& f) : x(static_cast<int>(f.x)), y(static_cast<int>(f.y)) {}
	operator ImVec2() const { return ImVec2(static_cast<float>(x), static_cast<float>(y)); }
};

// for 4 edges, int size
struct QWPOINT {
	int bottom = 0;
	int left = 0;
	int right = 0;
	int top = 0;

	QWPOINT() = default;
	QWPOINT(int _bottom, int _left, int _right, int _top) :
		bottom{_bottom}, left{_left}, right{_right}, top{_top} {}
};

struct MoveSpeed {
	char CurrentWalk = -1;//walk tiles left nr
	FFPOINT CurrentTile{};//maybe
	std::vector<FFPOINT> Tiles{};//tiles left to walk, x,y norm tiles. z as speed -1,1,2 stand, walk, run
};

//def special struct for gates
struct GateTileStruct {
	std::string name{};//gate name
	FFPOINT start_tile = { 0,0,0 };//in front
	int object = 0;// if null then skip
	FFPOINT end_tile = { 0,0,0 };//use z as floor
	int normal_object_offset = 0;
	bool walkmanually = false;
	bool objectdone = false;
	bool ignore_endtile = false;//ignore end
	int direction = 0;//1 left, 2 right, 3 down, 4 up, 5 around
	std::vector<int> gatetiles_ids{};//door ids, for finding room
	std::vector<FFPOINT> gatetiles_doors{};//door tiles, for finding room
	std::vector<FFPOINT> gatetiles_npclocation{};//aprox wander, for finding room
	int npc = 0;//id, for finding room
	int npc_wander_area = 0;//for finding room
	int special_item_use = 0;//use item on object puzzle
	int special_object_use = 0;//use item on object puzzle, if alone also just click
	int special_object_offset = 0;//use item on object puzzle, action offset
	bool special_object_done = false;//use item on object puzzle, if object done then change to true
	bool special_OBJon = false;//use item on object puzzle, do
	bool special_itemuseon = false;//use item on object puzzle, do use item
	bool gateisnpc = false;//npc teleport
	std::vector<int> pick_options{};//options from chat, just punch them in
	FFPOINT special_tile{};//final-final tile
	int sometimesblockage = 0;//Npc is sometimes blocked by object. check if npc is in spot, object id, start_tile vs npc vs sometimesblockage
};

// MAPLINK: moving/teleport objects that move the player to another part of the
// world. objectTile is the exact tile the object occupies (the tile used to
// verify the player really moved away after using it).
//   MapLink_Classic: the object (door/gate) is opened and the player has to walk
//     past it. The star planner makes the object tile passable and crosses it.
//   MapLink_Teleport: using openAction teleports the player (or moves it far
//     away). exitTile is optional knowledge; {0,0,0} means unknown - the
//     executor uses the link and re-plans from wherever the player lands.
enum MapLinkKind {
	MapLink_Classic = 0,
	MapLink_Teleport = 1
};

struct MapLink {
	int objectId = 0;
	std::string name{};//object name as it appears in the loc cache
	WPOINT objectTile{};//exact object tile, z = plane
	std::string openAction{};//action to open / use / enter (force teleport)
	std::string closedAction{};//action to close (classic doors)
	int kind = MapLink_Classic;
	WPOINT exitTile{};//known landing tile for teleports, {0,0,0} = unknown
	int walkDistance = 6;//how close the player must be to interact
	int extraCost = 0;//added to the star path cost when comparing routes
	bool closeAfter = false;//classic only: close the door after passing
};

//result of DO::StarPathPlan / DO::StarPathFind
struct StarPathResult {
	bool found = false;//a complete route to the goal is known
	bool direct = false;//route is a plain walk (may include classic door links)
	bool used_classic_link = false;//path crosses at least one classic MAPLINK tile
	int cost = -1;//tile-cost sum (diagonal = 14, straight = 10)
	std::vector<WPOINT> path{};//full path, or path to the link for a fallback
	int map_link_index = -1;//index into MEX::MapLinks() when a link is used/attempted
	int map_link_object = 0;//object id of that link
	int link_path_index = -1;//path index where the teleport happens (-1 for classic)
	bool map_link_destination_unknown = false;//teleport landing not known in advance
	bool map_link_fallback = false;//no full route; path only leads to a link
	std::string report{};//human readable planning result
};

// clue
struct clue_arrow {
	WPOINT clue_point{};
	std::string clue_string{};
	int size_count = 0;
};

// for varbb
struct VB {
	int state = 0;
	uint64_t addr = 0;
	uint64_t indexaddr_orig = 0;
	int id = 0;
	bool valid = false;
	bool found = false;

	VB() = default;
	VB(int _state, uint64_t _addr, uint64_t _indexaddr_orig, int _id, char array) :
		state{_state}, addr{_addr}, indexaddr_orig{_indexaddr_orig}, id{_id} {}
};

//for RefVarPlayer return
struct VBreturn {
	uint64_t SettingsAddr = 0;
	uint64_t SettingsAddrspot = 0;
	int SettingsState = 0;
	int SettingsId = -1;
	bool SettingsValid = false;
};

//for dung
struct DungMap {
	std::string text{};
	int varp = 0;
};

//universal struct for npcs/objects/players
struct AllObject {
	uint64_t Mem = 0;
	uint64_t MemE = 0;
	float TileX = 0.f;//raw tile
	float TileY = 0.f;//raw tile
	float TileZ = 0.f;//raw
	int Id = 0;
	int Life = 0;
	int Anim = 0;//animation on npc/player/on item noted bool
	std::string Name{};
	std::string Action{};
	int Floor = 0;
	uint64_t Amount = 0;
	int Type = 0;
	int Bool1 = 0;
	int ItemIndex = 0;//item index at Parsed_ItemsL
	int ViewP = 0;
	int ViewF = 0;
	float Distance = 0.f;
	int Cmb_lv = 0;
	int Unique_Id = 0;
	int CalcX = 0;
	int CalcY = 0;
	FFPOINT Tile_XYZ{};//norm tile x & y
	WPOINT Pixel_XYZ{};//norm pixels
	std::vector<WPOINT> Hitsplats{};
	int StatusBar1 = -1;
	int StatusBar2 = -1;
	std::vector<FFPOINT> MovingPoints{};
	WPOINT Head_XYZ{};//canvas position above actor head (for icons)
	jobject tileObject = nullptr;//must be global ref
	jclass objClass = nullptr;//must be global ref
	FFPOINT Start_Tile_XYZ{};//mostly for projectiles, start
	FFPOINT End_Tile_XYZ{};//mostly for projectiles, end
	uint64_t Time = 0;
	int pray = 0;
	//debug-only address mirrors for the map-data tile passes (types 6/99), emitted
	//by botmcp get_entities as linkMem/solidMem/cutMem and readable with
	//mem_read/mem_read_bytes:
	//  LinkMem  = raw walk cell link byte address of the tile (survives the wall
	//             overlay that overwrites Mem with a wall record)
	//  SolidMem = wall record address that disabled the tile (type 99)
	//  CutMem   = wall record address that cut walk directions (type 99)
	uint64_t LinkMem = 0;
	uint64_t SolidMem = 0;
	uint64_t CutMem = 0;

	AllObject() = default;
	AllObject(uint64_t _Mem, uint64_t _MemE, float _TileX, float _TileY, float _TileZ, int _Id, int _Life, int _Anim,
		std::string _Name, std::string _Action, int _Floor, uint64_t _Amount, int _Type, int _Bool1, int _ItemIndex, int _ViewP, int _ViewF,
		float _Distance, int _Cmb_lv, int _Unique_Id, int _CalcX, int _CalcY, FFPOINT _Tile_XYZ, WPOINT _Pixel_XYZ, std::vector<WPOINT> _Hitsplats, char _StatusBar1,
		char _StatusBar2, std::vector<FFPOINT> _MovingPoints) :
		Mem{_Mem}, MemE{_MemE}, TileX{_TileX}, TileY{_TileY}, TileZ{_TileZ}, Id{_Id}, Life{_Life}, Anim{_Anim}
		, Name{_Name}, Action{_Action}, Floor{_Floor}, Amount{_Amount}, Type{_Type}, Bool1{_Bool1}, ItemIndex{_ItemIndex}, ViewP{_ViewP}, ViewF{_ViewF}
		, Distance{ _Distance }, Cmb_lv{ _Cmb_lv }, Unique_Id{ _Unique_Id }, CalcX{ _CalcX }, CalcY{ _CalcY }, Tile_XYZ{ _Tile_XYZ }, Pixel_XYZ{ _Pixel_XYZ }, Hitsplats{ _Hitsplats }, StatusBar1{ _StatusBar1 },
		StatusBar2{ _StatusBar2 }, MovingPoints{ _MovingPoints }
	{}
};

//for interface stuff
struct InterfaceComp5 {
	int id1 = 0;
	int id2 = 0;
	int id3 = 0;
	uint64_t memloc = 0;

	InterfaceComp5() = default;
	InterfaceComp5(int _id1, int _id2, int _id3, uint64_t _memloc) :
		id1{_id1}, id2{_id2}, id3{_id3}, memloc{_memloc} {}
};

/// <summary>
/// Info about intefaces
/// </summary>
struct IInfo {
	int x = 0;//x start
	int xs = 0;//x size
	int y = 0;//y start
	int ys = 0;//y size
	int box_x = 0;//box size, generally empty
	int box_y = 0;//box size, genreally empty
	int scroll_y = 0;//for scrolling
	int id1 = 0;
	int id2 = 0;
	int id3 = 0;
	InterfaceComp5 firstIF = { 0,0,0,0 };
	int itemid1 = 0;
	uint64_t itemid1_size = 0;
	int itemid2 = 0;
	int slideSpriteId = 0;
	int buffIconId = 0;
	int abilityEnabledRaw = 0;
	int selectedTabId = 0;
	bool hov = false;
	std::string textids{};
	std::string textitem{};
	std::string alternateText{};
	uint64_t memloc = 0;
	uint64_t memloctop = 0;
	int index = 0;//how many levels deep
	std::string fullpath{};
	std::string fullIDpath{};
	bool notvisible = false;
	int OP = 0;
	WPOINT xy = {0,0,0};//x and y pixels at point
	/*
	IInfo() = default;
	IInfo(int _x, int _xs, int _y, int _ys, int _id1, int _id2, int _id3, int _id4, int _itemid1, int _itemid1_size, int _itemid2, bool _hov,
		std::string _textids, std::string _textitem, uint64_t _memloc, uint64_t _memloctop, int _index, std::string _fullpath, std::string _fullIDpath, bool _notvisible, int _OP, WPOINT _xy) :
		x{ _x }, xs{ _xs }, y{ _y }, ys{ _ys }, id1{ _id1 }, id2{ _id2 }, id3{ _id3 }, id4{ _id4 }, itemid1{ _itemid1 }, itemid1_size{ _itemid1_size }, hov{ _hov }
		, textids{ _textids }, textitem{ _textitem }, memloc{ _memloc }, memloctop{ _memloctop }, index{ _index }, fullpath{ _fullpath }, fullIDpath{ _fullIDpath }
		, notvisible{ _notvisible }, OP{ _OP }, xy{ _xy }
	{}
	*/
};

//LootOptions for configurable loot behavior
struct LootOptions {
	bool allowStackablesAlreadyHeld = false;
	bool allowCoins = false;
	int coinMinAmount = 300;
	int sleepOffset = 500;
	int maxItemsToLoot = 0;  // 0 = unlimited (loot all), 1 = one item, N = up to N items
	bool useLootAll = true;
	bool lootAmmoSlot = false;  // auto-loot items matching equipped ammo slot (e.g., arrows)
	bool useKeybindToOpen = false;
	int keybindVK = 0;  // VK code for keybind to open loot window
	bool lootAllWhenFull = false;  // use loot all when inventory full and there are stackables to loot
	bool dbg = false;
};

//for items, generally inventory
struct inv_Container_struct {
	int item_id = 0;
	int item_stack = 0;
	int item_slot = 0;
	std::string item_name{};
	int item_cat = 0;
	int item_gelimit = 0;
	int item_highalch = 0;
	bool item_noted = false;
	bool item_stackable = false;
	bool item_tradeable = false;
	bool item_bankable = false;
	bool item_alchable = false;
	WPOINT item_interface{};//for interface
	WPOINT item_xyz{};//for pixels
	std::vector<int> Pmap{};
	std::vector<uint64_t> Extra_mem{};
	std::vector<int> Extra_ints{};

	inv_Container_struct() = default;

	inv_Container_struct(
		int _item_id,
		int _item_stack,
		int _item_slot,
		const std::string& _item_name,
		int _item_cat,
		int _item_gelimit,
		int _item_highalch,
		bool _item_noted,
		bool _item_stackable,
		bool _item_tradeable,
		bool _item_bankable,
		WPOINT _item_interface,
		WPOINT _item_xyz,
		const std::vector<int>& _Pmap,
		const std::vector<uint64_t>& _Extra_mem,
		const std::vector<int>& _Extra_ints
	)
		: item_id(_item_id),
		item_stack(_item_stack),
		item_slot(_item_slot),
		item_name(_item_name),
		item_cat(_item_cat),
		item_gelimit(_item_gelimit),
		item_highalch(_item_highalch),
		item_noted(_item_noted),
		item_stackable(_item_stackable),
		item_tradeable(_item_tradeable),
		item_bankable(_item_bankable),
		item_interface(_item_interface),
		item_xyz(_item_xyz),
		Pmap(_Pmap),
		Extra_mem(_Extra_mem),
		Extra_ints(_Extra_ints)
	{
	}
};

struct General_Container {
	int id = 0;//somekind of container id
	uint64_t mem = 0;//
	std::vector<inv_Container_struct> ID_stack;//item id, stacksize, order, extra, All data is read by at once by GetContainerSettings()
	General_Container() = default;
	General_Container(int _id, inv_Container_struct _ID_stack) :
		id{ _id }, ID_stack{ _ID_stack } {
	}
};

//for interface stuff
struct InterfaceComp5test {
	std::string Name{};
	InterfaceComp5 IDdynamic{};
};

//for chooseoption
struct Choption {
	int Id = 0;
	int OPr = 0;
	int Index = 0;
	std::string Action{};
	std::string Name{};
	std::vector<InterfaceComp5> Randomcomponents{};
	QWPOINT Corners = {0,0,0,0};
};

//FindChooseOptionLive
struct ChOpt {
	uint64_t SummAddress = 0;
	int SummAmount = 0;
};

//Buffbar
struct Bbar {
	int id = 0;
	bool found = false;
	std::string text{};
	int conv_text = 0;
	uint64_t HexAddress = 0;//probl not needed to be exported

	Bbar() = default;
	Bbar(int _id, bool _found, std::string _text, int _conv_text) :
		id{_id}, found{_found}, text{_text}, conv_text{_conv_text} {}
};

//Target buff/debuff with detailed info
struct TargetBuff {
	int id = 0;              // Varbit value (stack count, etc.)
	int varbitId = 0;        // Varbit ID
	int spriteId = 0;        // Sprite/icon ID (BuffID)
	std::string name{};      // Buff name
	bool isDebuff = false;   // true = debuff, false = buff

	TargetBuff() = default;
	TargetBuff(int _id, int _varbitId, int _spriteId, std::string _name, bool _isDebuff) :
		id{_id}, varbitId{_varbitId}, spriteId{_spriteId}, name{_name}, isDebuff{_isDebuff} {}
};

struct Target_data {
	std::string Target_Name{};//name
	int Hit_percent = 0;//
	int Cmb_lv = 0;//
	int Hitpoints = 0;//
	std::vector<TargetBuff> Buff_stack;//All buffs/debuffs

	Target_data() = default;
	Target_data(std::string _Target_Name, int _Hit_percent, int _Cmb_lv, int _Hitpoints, std::vector<TargetBuff> _Buff_stack) :
		Target_Name{ _Target_Name }, Hit_percent{ _Hit_percent }, Cmb_lv{ _Cmb_lv }, Hitpoints{ _Hitpoints }, Buff_stack{ _Buff_stack } {
	}
};

//for Abilities
struct Abilitybar {
	int slot = 0;//slot
	int id = 0;//id
	std::string name{};//ability name
	unsigned char hotkey = 0;//key
	int cooldown_timer = 0;//only when it turned on in settings
	IInfo info{};//might be needed for doaction. Some extra info can be stored here
	std::string action{};//action
	bool enabled = false;//sometimes shows if button is enabled
	int modkey = 0;//1 shift, 2 ctrl, 3 alt
	int bar_nr = -1;

	Abilitybar() = default;
	Abilitybar(int _slot, int _id, std::string _name, unsigned char _hotkey, int _cooldown_timer, IInfo _info, std::string _action, bool _enabled, int _modkey) :
		slot{ _slot }, id{ _id }, name{ _name }, hotkey{ _hotkey }, cooldown_timer{ _cooldown_timer }, info{ _info }, enabled{ _enabled }, modkey{ _modkey } {}
};

// holds details about skills. id, name, varbit, total xp, current level, boosted level
struct Skill {
	int interfaceIdx;
	int id;
	std::string name{};
	int vb;
	int xp;
	int level;
	int boostedLevel;
	Skill() = default;
	Skill(int _idx = -1, int _id = -1, std::string _name = "", int _vb = -1, int _xp = -1, int _level = -1, int _boostedLevel = -1) :
		interfaceIdx{ _idx }, id { _id }, name{ _name }, vb{ _vb }, xp{ _xp }, level{ _level }, boostedLevel{ _boostedLevel } {}
};

struct EventData {
	std::string name{};
	std::string name2{};
	std::string name3{};
	std::string chat_type{};
	std::string text{};
	std::time_t timestamp1{};
	std::string timestamp2{};
	int timestamp3;
	int skillIndex;
	std::string skillName{};
	int exp;
	int ItemID;
	int ItemAM;
};

//input and output
struct IG_answer {
	std::string box_name{};//name
	FFPOINT box_start{ 0,0,0 };//box start
	FFPOINT box_size{ 0,0,0 };//box size
	ImColor colour = ImColor(0, 255, 128);//color
	float radius = 20.f;//circle size
	float thickness = 1.f;//outline thick
	int how_many_sec = 5;//how many sec to live
	bool box_ticked = false;//is box ticked
	bool return_click = false;//is box clicked on, must be manually turned off
	bool remove = false;//remove this interface
	int int_value = 0;//ffa
	uint64_t mem_local = 0;//ffa
	uint64_t mem_global = 0;//ffa
	std::string tooltip_text{};//ffa
	std::string string_input{};//ffa
	std::string string_value{};//ffa
	std::vector<std::string>stringsArr{};
	bool temp_created = false;//temp created by function by new, delete by new
};
//for ImGui processing
struct ImGui_loop_info {
	bool Test_f = false;
	std::function<bool()> function_info{};//pointer + packed arguments
	std::string unique_identifier{};//write or generate
	bool resident = false;//permanent until removed
	IG_answer* dat = nullptr;
};

struct PASSOBJ {
	uint64_t Data_pointer = 0;
	int Obj_tick = 0;
	uint64_t Obj_failure = 0;
};
