#pragma once

#include "ScriptLoader.h"
#include <vector>
#include <wtypes.h>
#include <string>
#include "imgui.h"
#include <API.h>

extern "C" __declspec(dllexport) void GetScripts(std::shared_ptr<ScriptFunction[]>*functions, size_t * count) {
    std::initializer_list<ScriptFunction> initList = {
       //{ "ClueTest_loop", &ClueTest_loop },
        { "Priff_summ", &Priff_summ },
        { "CombatTesting", &CombatTesting },
        { "FFA_fighter", &FFA_fighter },
        { "CitadelWoodChopper", &CitadelWoodChopper },
        { "TestScript", &TestScript },
        { "AutoDivination", &AutoDivination }
        //Add your scripts here
    };

    *count = initList.size();
    std::shared_ptr<ScriptFunction[]> functionArray(new ScriptFunction[*count]);
    std::copy(initList.begin(), initList.end(), functionArray.get());

    *functions = functionArray;
}

extern "C" __declspec(dllexport) void LoadDLL(HWND parent) {
    DWORD processId;
    GetWindowThreadProcessId(parent, &processId);
    std::string processIdStr = std::to_string(processId);
    MessageBoxA(nullptr, processIdStr.c_str(), "LoadDLL", MB_OK | MB_ICONERROR);
    return;
}

extern "C" __declspec(dllexport) void RenderUI() {
    ImGuiContext* ctx = GetImGuiContext();
    if (!ctx) {
        return;  // Exit if we can't get a valid context
    }
    ImGui::SetCurrentContext(ctx);
	ImGui::Begin("External Plugin UI");
    ImGui::Text("This is an external plugin UI example.");
	ImGui::Text("You can add your own UI elements here.");
    if(ImGui::Button("Unload Plugin")) {
        g_unloadRequested = true;
    }

    ImGui::End();
}

extern "C" __declspec(dllexport) void onUnload() {
    MessageBoxA(nullptr, "onUnload", "onUnload", MB_OK | MB_ICONERROR);
}

extern "C" __declspec(dllexport) bool UnloadRequested() {
    return g_unloadRequested;
}