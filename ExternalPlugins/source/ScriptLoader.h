#pragma once
#include <iostream>

static bool g_unloadRequested = false;
struct ScriptFunction {
    const char* name;
    void (*functionPointer)();
};

/// <summary>
/// Main script functions
/// </summary>

//void ClueTest_loop();
void Priff_summ();
void CombatTesting();
void FFA_fighter();
void CitadelWoodChopper();
void TestScript();
void AutoDivination();
void CitadelWoodChopper();
