#pragma once
#include <iostream>

static bool g_unloadRequested = false;
struct ScriptFunction {
    const char* name;
    void (*functionPointer)();
    const char* description;
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
void Example_LogOnce();
void Example_LoopyCounter();
void Example_TimedStatus();
void Example_RandomSleep();
void Example_SetStatusLines();
void Example_Pulse();
void Example_QuickExit();
void Example_Heartbeat();
void Example_TickPrinter();
void Example_CycleStates();
void Example_Backoff();
void Example_SafeStop();
void Example_SpamGuard();
void Example_OneShotIdle();
void Example_ProgressDemo();
void Example_IdlePing();
void Example_FakeTask();
void Example_ThresholdStop();
void Example_FlagFlip();
void Example_StopAfterDelay();
