#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "imgui.h"
//we store data

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

	ReturnText() = default;
	ReturnText(std::string _Name, int _Nr) : Name{ _Name }, Nr{ _Nr } {}
};

//
struct ChatTexts {
	std::string name{};
	std::string text{};
	std::string text_extra1{};
	std::string text_extra2{};
	uint64_t mem_loc = 0;
	uint64_t pc_time_stamp = 0;
	size_t pos_found = 0;
	uint64_t time_total = 0;
};

// player name info
struct NameData {
	std::string Name{};
	std::string Password{};
	std::string Pin{};
};

// for keyboard
struct KeyboardKeys {
	unsigned char virtual_keys = 0;
	unsigned char scan_keys = 0;
	bool extended = 0;
	unsigned char ascii = 0;
};

// for coordinates
struct FFPOINT {
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;

	FFPOINT() = default;
	FFPOINT(float _x, float _y, float _z) : x{ _x }, y{ _y }, z{ _z } {}
};

// for coordinates, int size point
struct WPOINT {
	int x = 0;
	int y = 0;
	int z = 0;

	WPOINT() = default;
	WPOINT(int _x, int _y, int _z) : x{ _x }, y{ _y }, z{ _z } {}
	WPOINT(const FFPOINT& f) : x(f.x), y(f.y), z(f.z) {}

	operator FFPOINT() const { return FFPOINT(x, y, z); }
};

// for 4 edges, int size
struct QWPOINT {
	int bottom = 0;
	int left = 0;
	int right = 0;
	int top = 0;

	QWPOINT() = default;
	QWPOINT(int _bottom, int _left, int _right, int _top) :
		bottom{ _bottom }, left{ _left }, right{ _right }, top{ _top } {}
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
	int FSarray = 0;
	int Levels_deep = 0;

	VB() = default;
	VB(int _state, uint64_t _addr, uint64_t _indexaddr_orig, int _id, char array, char _FSarray, char _Levels_deep) :
		state{ _state }, addr{ _addr }, indexaddr_orig{ _indexaddr_orig }, id{ _id }, FSarray{ _FSarray }, Levels_deep{ _Levels_deep } {}
};


struct inv_Container_struct {
	int item_id = 0;
	int item_stack = 0;
	int item_slot = 0;
	std::vector<uint64_t> Extra_mem{};
	std::vector<int> Extra_ints{};
	inv_Container_struct() = default;
	inv_Container_struct(int _item_id, int _item_stack, int _item_slot, std::vector<uint64_t> _Extra_mem, std::vector<int> _Extra_ints) :
		item_id{ _item_id }, item_stack{ _item_stack }, item_slot{ _item_slot }, Extra_mem{ _Extra_mem }, Extra_ints{ _Extra_ints } {}
};

struct inv_Container {
	int id = 0;//somekind of container id
	uint64_t mem = 0;//
	std::vector<inv_Container_struct> ID_stack;//item id, stacksize, order, extra
	inv_Container() = default;
	inv_Container(int _id, inv_Container_struct _ID_stack) :
		id{ _id }, ID_stack{ _ID_stack } {}
};

//for RefVarpBits444 return
struct VBreturn {
	uint64_t SettingsAddr = 0;
	uint64_t SettingsAddrspot = 0;
	int SettingsState = 0;
	int SettingsId = 0;
	int FSarray = 0;//0-1
	int Levels_deep = 0;//0-1
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

	AllObject() = default;
	AllObject(uint64_t _Mem, uint64_t _MemE, float _TileX, float _TileY, float _TileZ, int _Id, int _Life, int _Anim,
		std::string _Name, std::string _Action, int _Floor, uint64_t _Amount, int _Type, int _Bool1, int _ItemIndex, int _ViewP, int _ViewF,
		float _Distance, int _Cmb_lv, int _Unique_Id, int _CalcX, int _CalcY, FFPOINT _Tile_XYZ, WPOINT _Pixel_XYZ) :
		Mem{ _Mem }, MemE{ _MemE }, TileX{ _TileX }, TileY{ _TileY }, TileZ{ _TileZ }, Id{ _Id }, Life{ _Life }, Anim{ _Anim }
		, Name{ _Name }, Action{ _Action }, Floor{ _Floor }, Amount{ _Amount }, Type{ _Type }, Bool1{ _Bool1 }, ItemIndex{ _ItemIndex }, ViewP{ _ViewP }, ViewF{ _ViewF }
		, Distance{ _Distance }, Cmb_lv{ _Cmb_lv }, Unique_Id{ _Unique_Id }, CalcX{ _CalcX }, CalcY{ _CalcY }, Tile_XYZ{ _Tile_XYZ }, Pixel_XYZ{ _Pixel_XYZ } {}
};

//for interface stuff
struct InterfaceComp5 {
	int id1 = 0;
	int id2 = 0;
	int id3 = 0;
	int id4 = 0;
	uint64_t memloc = 0;

	InterfaceComp5() = default;
	InterfaceComp5(int _id1, int _id2, int _id3, int _id4, uint64_t _memloc) :
		id1{ _id1 }, id2{ _id2 }, id3{ _id3 }, id4{ _id4 }, memloc{ _memloc } {}
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
	int id4 = 0;
	InterfaceComp5 firstIF = { 0,0,0,0,0 };
	int itemid1 = 0;
	uint64_t itemid1_size = 0;
	int itemid2 = 0;
	bool hov = false;
	std::string textids{};
	std::string textitem{};
	uint64_t memloc = 0;
	uint64_t memloctop = 0;
	int index = 0;//how many levels deep
	std::string fullpath{};
	std::string fullIDpath{};
	bool notvisible = false;
	int OP = 0;
	WPOINT xy = { 0,0,0 };//x and y pixels at point
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

//for interface stuff
struct InterfaceComp5test {
	std::string Name{};
	InterfaceComp5 IDdynamic{};
	std::vector<InterfaceComp5> IDstatics{};
};

//
struct IInfoArrs {
	IInfo IS0;
	std::vector<IInfo> IS1{};
	std::vector<IInfo> IS2{};
	std::vector<IInfo> IS3{};
	std::vector<IInfo> IS4{};
	std::vector<IInfo> IS5{};
	std::vector<IInfo> IS6{};
	std::vector<IInfo> IS7{};
	std::vector<IInfo> IS8{};
	std::vector<IInfo> IS9{};
	std::vector<IInfo> IS10{};
	std::vector<IInfo> IS11{};
	std::vector<IInfo> IS12{};
	std::vector<IInfo> IS13{};
	std::vector<IInfo> IS14{};
	std::vector<IInfo> IS15{};
};

//for chooseoption
struct Choption {
	std::string TextLines{};
	int x = 0;
	int y = 0;
	int w = 0;
	int h = 0;
	int index0 = 0;
	int index1 = 0;
	int index2 = 0;
	int itemid = 0;
	int objectid = 0;
	int stuff0 = 0;
	int stuff1 = 0;
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

	Bbar() = default;
	Bbar(int _id, bool _found, std::string _text, int _conv_text) :
		id{ _id }, found{ _found }, text{ _text }, conv_text{ _conv_text } {}
};

struct Abilitybar {
	int slot = 0;//slot
	int id = 0;//id
	std::string name{};//ability name
	char hotkey = 0;//key
	int cooldown_timer = 0;//only when it turned on in settings
	IInfo info{};//might be needed for doaction. Some extra info can be stored here
	std::string action{};//action
	bool enabled = false;//sometimes shows if button is enabled

	Abilitybar() = default;
	Abilitybar(int _slot, int _id, std::string _name, char _hotkey, int _cooldown_timer, IInfo _info, std::string _action, bool _enabled) :
		slot{ _slot }, id{ _id }, name{ _name }, hotkey{ _hotkey }, cooldown_timer{ _cooldown_timer }, info{ _info }, enabled{ _enabled } {}
};

// holds details about skills. id, name, varbit, total xp, current level, boosted level
struct Skill {
	int interfaceIdx;
	int id;
	std::string name;
	int vb;
	int xp;
	int level;
	int boostedLevel;
	Skill() = default;
	Skill(int _idx = -1, int _id = -1, std::string _name = "", int _vb = -1, int _xp = -1, int _level = -1, int _boostedLevel = -1) :
		interfaceIdx{ _idx }, id{ _id }, name{ _name }, vb{ _vb }, xp{ _xp }, level{ _level }, boostedLevel{ _boostedLevel } {}
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