#pragma once
#include "API.h"
#include "ScriptLoader.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <functional>

using std::string;
using std::vector;
using std::to_string;

// KappieArch - Convert from Lua to C++
// This script handles various archaeology and skilling activities in RuneScape

// Global variables
static string version = "0.1";
static int materialsExcavated = 0;
static int artifactsFound = 0;
static const int MAX_IDLE_TIME_MINUTES = 5;
static time_t startTime = std::time(nullptr);
static time_t lastXpTime = std::time(nullptr);
static time_t afk = std::time(nullptr);
static int targetNotFoundCount = 0;
static bool runLoop = false;
static string selectedTarget;
static int minutes = 0;
static int seconds = 0;
static int aggression = 0;

// GUI Data
static IG_answer imguiBackground{};
static IG_answer imguiCurrentTarget{};
static IG_answer imguiStart{};
static IG_answer imguiPause{};
static IG_answer imguiTerminate{};
static IG_answer imguiRuntime{};
static IG_answer imguiArtifactsFound{};
static IG_answer imguiNumberInput{};

// UI Colors
struct Colors {
    ImColor BACKGROUND = ImColor(165, 23, 69);
    ImColor TARGET_UNSET = ImColor(189, 185, 167);
    ImColor TARGET_SET = ImColor(70, 143, 126);
    ImColor EXCAVATE = ImColor(84, 166, 102);
    ImColor PAUSED = ImColor(238, 59, 83);
    ImColor RUNTIME = ImColor(198, 120, 102);
} COLORS;

// Object IDs
struct Objects {
    int FALADOR_LADDER_IN_CAVE = 6226;
    int FALADOR_LADDER_TO_CAVE = 2113;
    int FALADOR_BANK = 100247;
    int FALADOR_DOOR_IN_CAVE = 2112;
} OBJECTS;

// State variables for various activities
static bool isExcavatingMaterial = false;
static bool isLooting = false;
static bool isMining = false;
static bool isBanking = false;
static bool hasBanked = false;
static bool isSmithing = false;
static bool isHeating = false;
static bool isFletching = false;
static bool isTanning = false;
static bool hasTanned = false;
static bool isCrafting = false;
static bool hasCrafted = false;
static bool isDisassembling = false;
static bool isSmelting = false;
static bool isMakingJewelry = false;
static bool isWoodcutting = false;

// ID maps for various materials and spots
struct ExcavateMaterial {
    int hellfireMetal = 116426;
    int orgone = 118957;
    int quintessence = 116381;
    int vellum = 116406;
    int whiteMarble = 116414;
    int compassRose = 118952;
    int fossilisedBone = 116408;
    int dragonMetal = 118954;
    int orthenglass = 116404;
    int soapstone = 116410;
    int leatherScraps = 116405;
    int warforgeBronze = 116389;
    int imperialSteel = 116431;
    int everlightSilver = 116417;
    int ancientVis = 116432;
    int bloodOfOrcus = 116435;
    int felt = 118956;
} excavateMaterial;

struct ExcavationSpots {
    int fishingHut = 116675;
    int dragonkinCoffin = 119079;
    int kyzajChampionBoudior = 117375;
    int icyeneWeaponRack = 116679;
    int culinarumDebris = 117122;
    int autopsyTable = 119081;
    int experimentWorkbench = 119083;
} excavationSpots;

// Utility Functions
string formatElapsedTime(time_t start) {
    time_t currentTime = std::time(nullptr);
    long elapsedTime = static_cast<long>(std::difftime(currentTime, start));
    int hours = static_cast<int>(elapsedTime / 3600);
    minutes = static_cast<int>((elapsedTime % 3600) / 60);
    seconds = static_cast<int>(elapsedTime % 60);

    std::ostringstream oss;
    oss << "Runtime: " << std::setw(2) << std::setfill('0') << hours << ":"
        << std::setw(2) << std::setfill('0') << minutes << ":"
        << std::setw(2) << std::setfill('0') << seconds;
    return oss.str();
}

void idleCheck() {
    time_t currentTime = std::time(nullptr);
    time_t timeDiff = static_cast<time_t>(std::difftime(currentTime, afk));
    time_t randomTime = (rand() % static_cast<int>((MAX_IDLE_TIME_MINUTES * 60) * 0.3))
        + static_cast<int>((MAX_IDLE_TIME_MINUTES * 60) * 0.6);

    if (timeDiff > randomTime) {
        ME::PIdle2();
        afk = currentTime;
    }
}

bool gameStateChecks() {
    int gameState = ME::GetGameState2();
    if (gameState != 3) {
        DebugImGui.Debugtext_add("Not ingame with state: " + to_string(gameState));
        LoopyLoop = false;
        return false;
    }

    if (targetNotFoundCount > 30) {
        imguiStart.box_name = "Start";
        runLoop = false;
        LoopyLoop = false;
        return false;
    }
    return true;
}

void terminate() {
    runLoop = false;
    LoopyLoop = false;
}

void pauseExcavation() {
    runLoop = false;
    DebugImGui.Debugtext_add("Script paused");
    imguiCurrentTarget.colour = COLORS.PAUSED;
    imguiRuntime.colour = COLORS.PAUSED;
}

void startExcavation() {
    DebugImGui.Debugtext_add("Script started");
    runLoop = true;
    imguiCurrentTarget.colour = COLORS.EXCAVATE;
    imguiRuntime.colour = COLORS.EXCAVATE;
}

// Core Functions for Different Activities
void bankInFalador() {
    if (!isBanking) {
        DebugImGui.Debugtext_add("Going to ladder in cave");
        DO::DoAction_Object(0x35, OFF_ACT::GeneralObject_route0, { OBJECTS.FALADOR_DOOR_IN_CAVE }, 50);
        MEX::WaitUntilMovingEnds(6, 50);
        DO::DoAction_Object(0x35, OFF_ACT::GeneralObject_route0, { OBJECTS.FALADOR_LADDER_IN_CAVE }, 50);
        MEX::WaitUntilMovingEnds(6, 50);
        isBanking = true;
    }

    DebugImGui.Debugtext_add("Going to bank");
    if (!hasBanked) {
        DO::DoAction_Object(0x33, OFF_ACT::GeneralObject_route3, { OBJECTS.FALADOR_BANK }, 50);
        MEX::WaitUntilMovingEnds(6, 50);
        if (ME::InvItemFound(44820) == false) {
            hasBanked = true;
        }
    }

    if (hasBanked) {
        DebugImGui.Debugtext_add("Going back to the caves");
        DO::DoAction_Object(0x35, OFF_ACT::GeneralObject_route0, { OBJECTS.FALADOR_LADDER_TO_CAVE }, 50);
        MEX::WaitUntilMovingEnds(6, 50);
        DO::DoAction_Object(0x35, OFF_ACT::GeneralObject_route0, { OBJECTS.FALADOR_DOOR_IN_CAVE }, 50);
        MEX::WaitUntilMovingEnds(6, 50);
        isMining = false;
        hasBanked = false;
        isBanking = false;
    }
}

void equipNewPorter() {
    Abilitybar porterAB = MEX::GetABs_name("Sign of the porter VII");
    DO::DoAction_Ability_Direct(porterAB, 2, OFF_ACT::GeneralInterface_route);
    ME::RandomSleep2(600, 1200, 0);
}

void checkForPorter() {
    Bbar porterBuff = MEX::Buffbar_GetIDstatus(51490, false);
    int portersFound = ME::InvItemcount_(51490);
    if (!porterBuff.found) {
        if (portersFound > 0) {
            equipNewPorter();
            ME::RandomSleep2(600, 1200, 0);
        }
    }
}

AllObject FindHl(vector<int> objects, int maxdistance, vector<int> highlight) {
    vector<AllObject> objObjs = ME::GetAllObjArray(objects, maxdistance, { 0 });
    vector<AllObject> hlObjs = ME::GetAllObjArray(highlight, maxdistance, { 4 });
    AllObject shiny{};

    for (float i = 0; i <= 1.5f; i += 0.1f) {
        for (const auto& obj : objObjs) {
            for (const auto& hl : hlObjs) {
                if (fabs(obj.Tile_XYZ.x - hl.Tile_XYZ.x) < i && fabs(obj.Tile_XYZ.y - hl.Tile_XYZ.y) < i) {
                    shiny = obj;
                }
            }
        }
    }
    return shiny;
}

void autoMats() {
    bool dialog = ME::Check_continue_Open();
    string target = "Keshik tower debris";
    vector<AllObject> foundObjects = ME::GetAllObjArray({ excavateMaterial.felt }, 60, { 12 });
    vector<int> targetIds;
    for (size_t i = 0; i < foundObjects.size(); i++) {
        AllObject rock = foundObjects[i];
        targetIds.push_back(rock.Id);
    }

    if (!isExcavatingMaterial) {
        DebugImGui.Debugtext_add("Excavating");
        DO::DoAction_Object_valid(0x2, OFF_ACT::GeneralObject_route0, targetIds, 50, true);
        isExcavatingMaterial = true;
        MEX::WaitUntilMovingEnds();
        ME::RandomSleep2(5000, 1200, 0);
    }

    if (dialog) {
        artifactsFound++;
        DO::DoAction_Object_valid(0x2, OFF_ACT::GeneralObject_route0, targetIds, 50, true);
        MEX::WaitUntilMovingEnds();
        ME::RandomSleep2(1200, 1200, 0);
    }

    if (!ME::CheckAnim(10) || MEX::isProcessing() || ME::ReadPlayerMovin2() && isExcavatingMaterial && ME::InvItemFound(51490)) {
        DebugImGui.Debugtext_add("Not excavating anymore");
        isExcavatingMaterial = false;
    }
}

void followSprite() {
    bool dialog = ME::Check_continue_Open();
    string target = "Keshik tower debris";
    vector<AllObject> foundObjects = ME::GetAllObjArray({ excavationSpots.experimentWorkbench }, 60, { 0 });
    vector<int> targetIds;

    for (size_t i = 0; i < foundObjects.size(); i++) {
        AllObject rock = foundObjects[i];
        targetIds.push_back(rock.Id);
    }

    AllObject sprite = FindHl(targetIds, 60, { 7307 });
    if (!ME::ReadPlayerMovin2()) {
        if (sprite.Id != 0) {
            WPOINT spritePos = { static_cast<int>(sprite.TileX / 512), static_cast<int>(sprite.TileY / 512), static_cast<int>(sprite.TileZ / 512) };
            float distanceF = ME::Math_DistanceF(ME::PlayerCoordfloat(), sprite.Tile_XYZ);

            if (distanceF > 1.6f) {
                ME::RandomSleep2(600, 1200, 0);
                if (!ME::CheckAnim(20) && foundObjects.size() > 0) {
                    DebugImGui.Debugtext_add("Excavating " + target);
                }
                else {
                    DebugImGui.Debugtext_add("Sprite has moved, chasing it");
                }
                DO::DoAction_Object(0x2, OFF_ACT::GeneralObject_route0, { sprite.Id }, 60, spritePos);
                ME::RandomSleep2(600, 1200, 0);
                MEX::WaitUntilMovingEnds();
                return;
            }

            if (dialog) {
                artifactsFound++;
                DO::DoAction_Object(0x2, OFF_ACT::GeneralObject_route0, { sprite.Id }, 60, spritePos);
                ME::RandomSleep2(600, 1200, 0);
                MEX::WaitUntilMovingEnds();
            }
        }
    }
}

void rockertunities() {
    bool dialog = ME::Check_continue_Open();
    string target = "Luminite";
    //vector<int> rocks = {113056, 113057, 113058}; // Luminite
    //vector<int> rocks = {113053, 113055, 113054}; // Adamantite
    vector<int> rocks = { 113067, 113066, 113065 }; // Runite
    vector<AllObject> foundObjects = ME::GetAllObjArray(rocks, 60, { 0 });
    vector<int> targetIds;

    for (size_t i = 0; i < foundObjects.size(); i++) {
        AllObject rock = foundObjects[i];
        targetIds.push_back(rock.Id);
    }

    AllObject sprite = FindHl(targetIds, 60, { 7164, 7165 });
    if (sprite.Id != 0 && (!ME::InvFull_() || ME::InvItemFound(29283))) {
        DebugImGui.Debugtext_add("rockertunity");
        WPOINT spritePos = { static_cast<int>(sprite.TileX / 512), static_cast<int>(sprite.TileY / 512), static_cast<int>(sprite.TileZ / 512) };
        DO::DoAction_Object(0x2, OFF_ACT::GeneralObject_route0, { sprite.Id }, 60, spritePos);
        ME::RandomSleep2(1200, 3600, 0);
        MEX::WaitUntilMovingEnds();
        return;
    }

    if (!isMining) {
        DebugImGui.Debugtext_add("startMining");
        isMining = true;
        DO::DoAction_Object(0x2, OFF_ACT::GeneralObject_route0, rocks, 15);
        ME::RandomSleep2(1200, 3600, 0);
        MEX::WaitUntilMovingEnds();
    }

    if (ME::LocalPlayer_HoverProgress() <= 30 && ME::LocalPlayer_HoverProgress() != 0 && isMining) {
        DebugImGui.Debugtext_add("stamina");
        DO::DoAction_Object(0x2, OFF_ACT::GeneralObject_route0, rocks, 15);
        ME::RandomSleep2(1200, 3600, 0);
    }

    if (ME::InvFull_() && !ME::InvItemFound(29283)) {
        DebugImGui.Debugtext_add("write bank route");
        pauseExcavation();
    }
}

int getProgress(int samples = 1) {
    int max = 0;
    for (int i = 0; i < samples; i++) {
        int progress = ME::LocalPlayer_HoverProgress();
        if (progress > max) max = progress;
        ME::RandomSleep2(250, 0, 0);
    }
    return max;
}

void heater() {
    DO::DoAction_Object(0x3f, OFF_ACT::GeneralObject_route0, { 120051 }, 50);
    isSmithing = false;
}

void switchToBreakDownHammer() {
    inv_Container_struct smithingProgress = ME::Container_Get_s(93, 47068);
    if (smithingProgress.Extra_ints[6] < 999) {
        ME::ClickInv_(39653, 0, 3); // Equip
        ME::RandomSleep2(600, 1200, 0);
        DO::DoAction_Object(0x3f, OFF_ACT::GeneralObject_route0, { 113262 }, 50);
        ME::RandomSleep2(600, 1200, 0);
        ME::ClickInv_(39653, 0, 3); // Equip
        DO::DoAction_Object(0x3f, OFF_ACT::GeneralObject_route0, { 113262 }, 50);
    }
}

void smither() {
    if (getProgress(5) < (rand() % 11 + 170) && !isHeating) {
        isHeating = true;
        heater();
    }

    if (!isSmithing) {
        DebugImGui.Debugtext_add("start smithing");
        isSmithing = true;
        isHeating = false;
        DO::DoAction_Object(0x3f, OFF_ACT::GeneralObject_route0, { 113268 }, 50);
        ME::RandomSleep2(600, 1200, 0);
    }
}

void autoFletch() {
    if (!isFletching) {
        isFletching = true;
        DO::DoAction_Object(0x29, OFF_ACT::GeneralObject_route0, { 106599 }, 50);
        ME::RandomSleep2(700, 600, 900);
        DO::DoAction_Interface(0xffffffff, 0xffffffff, 0, 1370, 30, -1, OFF_ACT::GeneralInterface_Choose_option);
        ME::RandomSleep2(11000, 600, 0);
        DebugImGui.Debugtext_add("Fletched");
        isFletching = false;
    }
}

void sleepTickRandom(int sleepticks) {
    MEX::Sleep_tick(sleepticks);
    ME::RandomSleep2(1, 240, 0);
}

static int item = 1;
void autoAlch() {
    vector<IInfo> inventoryItems = ME::ReadInvArrays33();
    if (inventoryItems.size() > 0 && inventoryItems[0].itemid1 == 561) {
        LoopyLoop = false;
        return;
    }

    Abilitybar AB = MEX::GetABs_name("High Level Alchemy");
    if (AB.id > 0 && AB.enabled) {
        DO::DoAction_Ability_Direct(AB, 0, OFF_ACT::Bladed_interface_route);
        DebugImGui.Debugtext_add("alching");
        ME::RandomSleep2(1, 240, 0);
    }

    if (inventoryItems.size() > item && inventoryItems[item].itemid1 == -1) {
        item++;
    }

    if (inventoryItems.size() > item) {
        DO::DoAction_Inventory(inventoryItems[item].itemid1, 0, 0, OFF_ACT::GeneralInterface_route1);
        MEX::Sleep_tick(4);
    }
}

void autoLoot() {
    if (seconds < 30 && isLooting) {
        isLooting = false;
    }

    int lootTick = seconds;
    if (ME::LootWindowOpen_2() && seconds - lootTick >= (rand() % 31 + 30) && !isLooting) {
        DebugImGui.Debugtext_add("There is loot");
        DO::DoAction_LootAll_Button();
        lootTick = 0;
        isLooting = true;
    }
}

void autoTan() {
    if (ME::InvItemFound(1751) && !isTanning) { // Blue dragonhide
        DO::DoAction_NPC(0x3e, OFF_ACT::InteractNPC_route, { 20281 }, 50);
        isTanning = true;
        isBanking = false;
        MEX::Sleep_tick(0);
    }

    vector<IInfo> interfaceCheck = ME::ScanForInterfaceTest2Get(true, { {1371, 7, -1, 0} });
    if (interfaceCheck.size() > 0 && !hasTanned) {
        DebugImGui.Debugtext_add("tans");
        DO::DoAction_Interface(0xffffffff, 0xffffffff, 0, 1370, 30, -1, OFF_ACT::GeneralInterface_Choose_option);
        hasTanned = true;
        MEX::Sleep_tick(0);
    }

    if (ME::InvItemFound(1753) && !isBanking) { // Blue dragonleather
        DO::DoAction_Object(0x33, OFF_ACT::GeneralObject_route3, { 92692 }, 50);
        isBanking = true;
        isTanning = false;
        hasTanned = false;
        MEX::Sleep_tick(0);
    }
}

void autoCraft() {
    if (ME::InvItemFound(64) && !isCrafting) {
        DebugImGui.Debugtext_add("Go to Craft Interface");
        DO::DoAction_Inventory(64, 0, 1, OFF_ACT::GeneralInterface_route);
        isCrafting = true;
        isBanking = false;
        MEX::Sleep_tick(2);
    }

    vector<IInfo> interfaceCheck = ME::ScanForInterfaceTest2Get(true, { {1371, 7, -1, 0} });
    if (interfaceCheck.size() > 0 && !hasCrafted) {
        DebugImGui.Debugtext_add("Begin Crafting");
        DO::DoAction_Interface(0xffffffff, 0xffffffff, 0, 1370, 30, -1, OFF_ACT::GeneralInterface_Choose_option);
        hasCrafted = true;
        MEX::Sleep_tick(2);
    }

    vector<IInfo> interfaceItems = ME::ScanForInterfaceTest2Get(false, { {1251, 8, -1, 0}, {1251, 36, -1, 0}, {1251, 0, -1, 0} });
    if (!isBanking && interfaceItems.size() > 0 && interfaceItems[0].itemid1 != 853) {
        DebugImGui.Debugtext_add("Banking");
        DO::DoAction_Object(0x33, OFF_ACT::GeneralObject_route3, { 125734 }, 50);
        isBanking = true;
        isCrafting = false;
        hasCrafted = false;
        MEX::Sleep_tick(3);

        if (!ME::InvItemFound(64)) {
            terminate();
        }
    }
}

void autoDisassemble() {
    Abilitybar disassembleAB = MEX::GetABs_name("Disassemble");
    if (!isDisassembling) {
        DO::DoAction_Ability("Disassemble", 0, OFF_ACT::Bladed_interface_route);
        MEX::Sleep_tick(0);
        DO::DoAction_Inventory(26814, 0, 0, OFF_ACT::GeneralInterface_route1);
        isDisassembling = true;
        DebugImGui.Debugtext_add("Disassembling");
        MEX::Sleep_tick(2);
    }

    vector<IInfo> interfaceItems = ME::ScanForInterfaceTest2Get(false, { {1251, 8, -1, 0}, {1251, 36, -1, 0}, {1251, 0, -1, 0} });
    if (interfaceItems.size() > 0 && interfaceItems[0].itemid1 != 13281) {
        isDisassembling = false;
        DebugImGui.Debugtext_add("Begin Disassembling");
    }
}

void autoSmelt() {
    if (!isSmelting) {
        DO::DoAction_Object(0x3f, OFF_ACT::GeneralObject_route0, { 113266 }, 50);
        MEX::Sleep_tick(2);
        DO::DoAction_Interface(0x24, 0xffffffff, 1, 37, 163, -1, OFF_ACT::GeneralInterface_route);
        isSmelting = true;
        DebugImGui.Debugtext_add("Smelting");
        MEX::Sleep_tick(2);
    }

    vector<IInfo> interfaceItems = ME::ScanForInterfaceTest2Get(false, { {1251, 8, -1, 0}, {1251, 36, -1, 0}, {1251, 0, -1, 0} });
    if (interfaceItems.size() > 0 && interfaceItems[0].itemid1 != 2357) {
        isSmelting = false;
        DebugImGui.Debugtext_add("Begin Smelting");
    }
}

void autoWeavePorters() {
    if (ME::InvItemFound(1664) && !isCrafting) {
        DebugImGui.Debugtext_add("Go to Craft Interface");
        DO::DoAction_Inventory(29324, 0, 1, OFF_ACT::GeneralInterface_route);
        isCrafting = true;
        isBanking = false;
        MEX::Sleep_tick(0);
    }

    vector<IInfo> interfaceCheck = ME::ScanForInterfaceTest2Get(true, { {1371, 7, -1, 0} });
    if (interfaceCheck.size() > 0 && !hasCrafted) {
        DebugImGui.Debugtext_add("Begin Crafting");
        DO::DoAction_Interface(0xffffffff, 0xffffffff, 0, 1370, 30, -1, OFF_ACT::GeneralInterface_Choose_option);
        hasCrafted = true;
        MEX::Sleep_tick(0);
    }

    if (!ME::InvItemFound(1664) && !isBanking) {
        DebugImGui.Debugtext_add("Banking");
        DO::DoAction_Object(0x33, OFF_ACT::GeneralObject_route3, { 125115 }, 50);
        isBanking = true;
        isCrafting = false;
        hasCrafted = false;
        MEX::Sleep_tick(0);
    }
}

void autoWoodcut() {
    int woodBox = 54911;
    int acadiaLog = 40285;
    int bankChest = 107493;
    int acadiaTree = 109001;
    int treeStump = 109002;

    vector<AllObject> acadiaTrees = ME::GetAllObjArray({ acadiaTree }, 60, { 0 });
    vector<FFPOINT> theGoodTrees;

    for (const auto& tree : acadiaTrees) {
        if (tree.Bool1 == 0) {
            theGoodTrees.push_back(tree.Tile_XYZ);
        }
    }

    if (!isWoodcutting && !theGoodTrees.empty()) {
        WPOINT treePos = { static_cast<int>(theGoodTrees[0].x), static_cast<int>(theGoodTrees[0].y), static_cast<int>(theGoodTrees[0].z) };
        DO::DoAction_Object(0x3B, OFF_ACT::GeneralObject_route0, { acadiaTree }, 50, treePos);
        DebugImGui.Debugtext_add("Cutting logs");
        ME::RandomSleep2(1200, 100, 200);
        isWoodcutting = true;
        isBanking = false;
        MEX::Sleep_tick(0);
    }

    vector<AllObject> stumps = ME::GetAllObjArray({ treeStump }, 2, { 0 });
    if (stumps.size() > 0) {
        DebugImGui.Debugtext_add("Tree died");
        isWoodcutting = false;
        MEX::Sleep_tick(0);
    }

    if (!ME::InvItemFound(29283) && !isBanking) {
        DebugImGui.Debugtext_add("No more porters");
        DO::DoAction_Object(0x33, OFF_ACT::GeneralObject_route3, { bankChest }, 60);
        MEX::WaitUntilMovingEnds(20, 4);
        isBanking = true;
        isWoodcutting = false;
    }
}

void initializeUI() {
    // Background
    imguiBackground.box_name = "imguiBackground";
    imguiBackground.box_start = { 16, 1400, 0 };
    imguiBackground.box_size = { 200, 1516, 0 };
    imguiBackground.colour = ImColor(165, 23, 69, 150);

    // Current Target
    imguiCurrentTarget.box_name = "Current Target:";
    imguiCurrentTarget.box_start = { 30, 50, 0 };

    // Start Button
    imguiStart.box_name = "Start";
    imguiStart.box_start = { 30, 1420, 0 };
    imguiStart.box_size = { 80, 30, 0 };
    imguiStart.tooltip_text = "Start Script";

    // Pause Button
    imguiPause.box_name = "Pause";
    imguiPause.box_start = { 100, 1420, 0 };
    imguiPause.box_size = { 80, 30, 0 };
    imguiPause.tooltip_text = "Pause Script";

    // Terminate Button
    imguiTerminate.box_name = "Stop Script";
    imguiTerminate.box_start = { 30, 1455, 0 };
    imguiTerminate.box_size = { 140, 30, 0 };
    imguiTerminate.tooltip_text = "Exit the script";

    // Runtime Text
    imguiRuntime.box_name = "imguiRuntime";
    imguiRuntime.box_start = { 30, 1500, 0 };

    // Artifacts Found Text
    imguiArtifactsFound.box_name = "imguiArtifactsFound";
    imguiArtifactsFound.box_start = { 30, 1485, 0 };
    imguiArtifactsFound.colour = ImColor(198, 120, 102);

    // Number Input
    imguiNumberInput.box_name = "Number Input";
    imguiNumberInput.box_start = { 30, 1510, 0 };
    imguiNumberInput.box_size = { 140, 30, 0 };
    imguiNumberInput.tooltip_text = "Enter a number";
    imguiNumberInput.string_input = "";
}

void drawGUI() {
    if (imguiTerminate.return_click) {
        imguiTerminate.return_click = false;
        terminate();
    }

    if (imguiStart.return_click) {
        imguiStart.return_click = false;
        if (!runLoop) {
            startExcavation();
        }
    }

    if (imguiPause.return_click) {
        imguiPause.return_click = false;
        if (runLoop) {
            pauseExcavation();
        }
    }

    IG::DrawSquareFilled(false, &imguiBackground);

    if (runLoop) {
        imguiRuntime.string_value = formatElapsedTime(startTime);
    }

    imguiArtifactsFound.string_value = "Artifacts Found: " + to_string(artifactsFound);

    IG::DrawBox(false, &imguiStart);
    IG::DrawBox(false, &imguiPause);
    IG::DrawBox(false, &imguiTerminate);
    IG::DrawTextAt(false, &imguiRuntime);
    IG::DrawTextAt(false, &imguiArtifactsFound);
}

// Main script function
void KappieArch() {
    DebugImGui.Debugtext_add("Started KappieArch " + version);
    startTime = std::time(nullptr);
    lastXpTime = std::time(nullptr);
    afk = std::time(nullptr);

    // Initialize UI components
    initializeUI();

    // Drawing function for ImGui
    auto drawImGuiFunc = []() {
        drawGUI();
        };

    // Register the drawing function
    DrawImGui(drawImGuiFunc);

    while (LoopyLoop) {
        // Handle basic events
        DO::RandomEvents();

        // Check game state
        if (!gameStateChecks()) {
            break;
        }

        // Anti-idle check
        idleCheck();

        // Only run activities if script is in active state
        if (runLoop) {
            // Check for signs of the porter
            checkForPorter();

            // Main activity - choose one or more based on what you want to do
            // Uncomment the activity you want to run

            // Archaeology activities
            // autoMats();
            followSprite();

            // Mining activities
            // rockertunities();

            // Smithing activities
            // smither();

            // Other skilling activities
            // autoFletch();
            // autoAlch();
            // autoTan();
            // autoCraft();
            // autoDisassemble();
            // autoSmelt();
            // autoWoodcut();
            // autoWeavePorters();

            // Loot handling
            // autoLoot();
        }

        // Sleep to prevent high CPU usage
        ME::RandomSleep2(600, 800, 1200);
    }

    DebugImGui.Debugtext_add("Stopped ErnieAIO " + version);
}