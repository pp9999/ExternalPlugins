#pragma once
#include "API.h"
#include "ScriptLoader.h"
#include "ScriptRegistry.h"
#include <cstdint>
#include <string>
#include <sstream>

static void ExampleLog(const std::string& text) {
    console_text << "[Example] " << text << std::endl;
    DebugImGui.Debugtext_addstream();
}

static std::string FormatMs(uint64_t ms) {
    std::ostringstream oss;
    oss << ms << "ms";
    return oss.str();
}

void Example_LogOnce() {
    ExampleLog("LogOnce start");
    ScripCuRunning0 = "Example_LogOnce";
    ScripCuRunning1 = "One-shot example";
    ME::RandomSleep2(200, 400, 800);
    ExampleLog("LogOnce end");
}

void Example_LoopyCounter() {
    ExampleLog("LoopyCounter start");
    LoopyLoop = true;
    for (int i = 0; LoopyLoop && !Endall && i < 10; ++i) {
        ScripCuRunning1 = "Count: " + std::to_string(i);
        ExampleLog("Count tick " + std::to_string(i));
        ME::RandomSleep2(200, 400, 800);
    }
    ExampleLog("LoopyCounter end");
}

void Example_TimedStatus() {
    ExampleLog("TimedStatus start");
    LoopyLoop = true;
    uint64_t start = GetTickCount64();
    while (LoopyLoop && !Endall) {
        uint64_t elapsed = GetTickCount64() - start;
        ScripCuRunning1 = "Elapsed: " + FormatMs(elapsed);
        if (elapsed > 5000) {
            break;
        }
        ME::RandomSleep2(200, 400, 800);
    }
    ExampleLog("TimedStatus end");
}

void Example_RandomSleep() {
    ExampleLog("RandomSleep start");
    LoopyLoop = true;
    for (int i = 0; LoopyLoop && !Endall && i < 6; ++i) {
        ScripCuRunning1 = "Random pause " + std::to_string(i + 1);
        ME::RandomSleep2(150, 500, 1200);
    }
    ExampleLog("RandomSleep end");
}

void Example_SetStatusLines() {
    ExampleLog("SetStatusLines start");
    LoopyLoop = true;
    for (int i = 0; LoopyLoop && !Endall && i < 5; ++i) {
        ScripCuRunning0 = "Example_SetStatusLines";
        ScripCuRunning1 = "Step " + std::to_string(i + 1) + " of 5";
        ScripCuRunning2 = (i % 2 == 0) ? "Working" : "Waiting";
        ME::RandomSleep2(300, 600, 1000);
    }
    ExampleLog("SetStatusLines end");
}

void Example_Pulse() {
    ExampleLog("Pulse start");
    LoopyLoop = true;
    bool on = false;
    for (int i = 0; LoopyLoop && !Endall && i < 8; ++i) {
        on = !on;
        ScripCuRunning1 = on ? "Pulse: ON" : "Pulse: OFF";
        ExampleLog(ScripCuRunning1);
        ME::RandomSleep2(250, 500, 900);
    }
    ExampleLog("Pulse end");
}

void Example_QuickExit() {
    ExampleLog("QuickExit start");
    ScripCuRunning0 = "Example_QuickExit";
    ScripCuRunning1 = "Stopping immediately";
    LoopyLoop = false;
    ExampleLog("QuickExit end");
}

void Example_Heartbeat() {
    ExampleLog("Heartbeat start");
    LoopyLoop = true;
    for (int i = 0; LoopyLoop && !Endall && i < 5; ++i) {
        ExampleLog("Heartbeat " + std::to_string(i + 1));
        ME::RandomSleep2(800, 1200, 2000);
    }
    ExampleLog("Heartbeat end");
}

void Example_TickPrinter() {
    ExampleLog("TickPrinter start");
    LoopyLoop = true;
    uint64_t start = GetTickCount64();
    for (int i = 0; LoopyLoop && !Endall && i < 6; ++i) {
        uint64_t delta = GetTickCount64() - start;
        ExampleLog("Tick delta " + FormatMs(delta));
        ME::RandomSleep2(200, 400, 800);
    }
    ExampleLog("TickPrinter end");
}

void Example_CycleStates() {
    ExampleLog("CycleStates start");
    LoopyLoop = true;
    const char* states[] = { "Idle", "Walking", "Interacting", "Cooling down" };
    for (int i = 0; LoopyLoop && !Endall && i < 8; ++i) {
        ScripCuRunning1 = states[i % 4];
        ExampleLog("State: " + ScripCuRunning1);
        ME::RandomSleep2(300, 600, 1000);
    }
    ExampleLog("CycleStates end");
}

void Example_Backoff() {
    ExampleLog("Backoff start");
    LoopyLoop = true;
    int delay = 200;
    for (int i = 0; LoopyLoop && !Endall && i < 6; ++i) {
        ExampleLog("Backoff delay " + std::to_string(delay) + "ms");
        ME::RandomSleep2(delay, delay + 100, delay + 300);
        if (delay < 1600) {
            delay *= 2;
        }
    }
    ExampleLog("Backoff end");
}

void Example_SafeStop() {
    ExampleLog("SafeStop start");
    LoopyLoop = true;
    for (int i = 0; LoopyLoop && !Endall && i < 12; ++i) {
        ExampleLog("Working chunk " + std::to_string(i + 1));
        ME::RandomSleep2(150, 300, 700);
    }
    LoopyLoop = false;
    ExampleLog("SafeStop end");
}

void Example_SpamGuard() {
    ExampleLog("SpamGuard start");
    LoopyLoop = true;
    for (int i = 1; LoopyLoop && !Endall && i <= 20; ++i) {
        if (i % 5 == 0) {
            ExampleLog("SpamGuard tick " + std::to_string(i));
        }
        ME::RandomSleep2(100, 200, 400);
    }
    ExampleLog("SpamGuard end");
}

void Example_OneShotIdle() {
    ExampleLog("OneShotIdle start");
    ScripCuRunning1 = "Calling PIdle2";
    ME::PIdle2();
    ExampleLog("OneShotIdle end");
}

void Example_ProgressDemo() {
    ExampleLog("ProgressDemo start");
    LoopyLoop = true;
    for (int pct = 0; LoopyLoop && !Endall && pct <= 100; pct += 20) {
        ScripCuRunning1 = "Progress " + std::to_string(pct) + "%";
        ExampleLog(ScripCuRunning1);
        ME::RandomSleep2(250, 450, 900);
    }
    ExampleLog("ProgressDemo end");
}

void Example_IdlePing() {
    ExampleLog("IdlePing start");
    LoopyLoop = true;
    for (int i = 0; LoopyLoop && !Endall && i < 3; ++i) {
        ME::PIdle2();
        ExampleLog("Idle ping " + std::to_string(i + 1));
        ME::RandomSleep2(600, 900, 1400);
    }
    ExampleLog("IdlePing end");
}

void Example_FakeTask() {
    ExampleLog("FakeTask start");
    LoopyLoop = true;
    ScripCuRunning0 = "Example_FakeTask";
    ScripCuRunning1 = "Preparing";
    ME::RandomSleep2(300, 600, 900);
    if (LoopyLoop && !Endall) {
        ScripCuRunning1 = "Executing";
        ME::RandomSleep2(400, 800, 1200);
    }
    if (LoopyLoop && !Endall) {
        ScripCuRunning1 = "Finishing";
        ME::RandomSleep2(300, 600, 900);
    }
    ExampleLog("FakeTask end");
}

void Example_ThresholdStop() {
    ExampleLog("ThresholdStop start");
    LoopyLoop = true;
    int threshold = 5;
    for (int i = 0; LoopyLoop && !Endall; ++i) {
        ScripCuRunning1 = "Value " + std::to_string(i);
        if (i >= threshold) {
            LoopyLoop = false;
            break;
        }
        ME::RandomSleep2(200, 400, 800);
    }
    ExampleLog("ThresholdStop end");
}

void Example_FlagFlip() {
    ExampleLog("FlagFlip start");
    LoopyLoop = true;
    bool flag = false;
    for (int i = 0; LoopyLoop && !Endall && i < 6; ++i) {
        flag = !flag;
        ScripCuRunning1 = flag ? "Flag: true" : "Flag: false";
        ExampleLog(ScripCuRunning1);
        ME::RandomSleep2(200, 350, 700);
    }
    ExampleLog("FlagFlip end");
}

void Example_StopAfterDelay() {
    ExampleLog("StopAfterDelay start");
    LoopyLoop = true;
    ScripCuRunning1 = "Waiting 3 seconds";
    ME::RandomSleep2(900, 1200, 1500);
    ME::RandomSleep2(900, 1200, 1500);
    ME::RandomSleep2(900, 1200, 1500);
    LoopyLoop = false;
    ExampleLog("StopAfterDelay end");
}

static ScriptRegistrar g_register_Example_LogOnce(
    "Example_LogOnce",
    &Example_LogOnce,
    "Example: one-shot script that logs start and end.");
static ScriptRegistrar g_register_Example_LoopyCounter(
    "Example_LoopyCounter",
    &Example_LoopyCounter,
    "Example: counter loop with status updates.");
static ScriptRegistrar g_register_Example_TimedStatus(
    "Example_TimedStatus",
    &Example_TimedStatus,
    "Example: timed loop that updates elapsed status.");
static ScriptRegistrar g_register_Example_RandomSleep(
    "Example_RandomSleep",
    &Example_RandomSleep,
    "Example: loop with randomized sleep timings.");
static ScriptRegistrar g_register_Example_SetStatusLines(
    "Example_SetStatusLines",
    &Example_SetStatusLines,
    "Example: updates ScripCuRunning0/1/2.");
static ScriptRegistrar g_register_Example_Pulse(
    "Example_Pulse",
    &Example_Pulse,
    "Example: toggles a pulse state and logs.");
static ScriptRegistrar g_register_Example_QuickExit(
    "Example_QuickExit",
    &Example_QuickExit,
    "Example: immediate stop after a status set.");
static ScriptRegistrar g_register_Example_Heartbeat(
    "Example_Heartbeat",
    &Example_Heartbeat,
    "Example: periodic heartbeat logging.");
static ScriptRegistrar g_register_Example_TickPrinter(
    "Example_TickPrinter",
    &Example_TickPrinter,
    "Example: prints tick deltas using GetTickCount64.");
static ScriptRegistrar g_register_Example_CycleStates(
    "Example_CycleStates",
    &Example_CycleStates,
    "Example: cycles through simple state strings.");
static ScriptRegistrar g_register_Example_Backoff(
    "Example_Backoff",
    &Example_Backoff,
    "Example: exponential-ish backoff between iterations.");
static ScriptRegistrar g_register_Example_SafeStop(
    "Example_SafeStop",
    &Example_SafeStop,
    "Example: loop with Endall/LoopyLoop checks.");
static ScriptRegistrar g_register_Example_SpamGuard(
    "Example_SpamGuard",
    &Example_SpamGuard,
    "Example: logs only every few ticks.");
static ScriptRegistrar g_register_Example_OneShotIdle(
    "Example_OneShotIdle",
    &Example_OneShotIdle,
    "Example: calls PIdle2 once with logs.");
static ScriptRegistrar g_register_Example_ProgressDemo(
    "Example_ProgressDemo",
    &Example_ProgressDemo,
    "Example: progress updates from 0-100%.");
static ScriptRegistrar g_register_Example_IdlePing(
    "Example_IdlePing",
    &Example_IdlePing,
    "Example: idle ping loop using PIdle2.");
static ScriptRegistrar g_register_Example_FakeTask(
    "Example_FakeTask",
    &Example_FakeTask,
    "Example: three-step simulated task.");
static ScriptRegistrar g_register_Example_ThresholdStop(
    "Example_ThresholdStop",
    &Example_ThresholdStop,
    "Example: stops when threshold is reached.");
static ScriptRegistrar g_register_Example_FlagFlip(
    "Example_FlagFlip",
    &Example_FlagFlip,
    "Example: flips a local flag and logs.");
static ScriptRegistrar g_register_Example_StopAfterDelay(
    "Example_StopAfterDelay",
    &Example_StopAfterDelay,
    "Example: waits then stops.");
