#pragma once

#include "ScriptLoader.h"
#include "ScriptRegistry.h"
#include "IconsFontAwesome6.h"
#include <vector>
#include <wtypes.h>
#include <string>
#include <atomic>
#include <thread>
#include <cstring>
#include <cfloat>
#include "imgui.h"
#include "imgui_internal.h"
#include <API.h>

static void LogPluginMessage(const std::string& message, int warn_level = 0) {
    console_text << "[ExternalPlugins] " << message << std::endl;
    DebugImGui.Debugtext_addstream(warn_level);
}

static float CalcIconTextButtonWidth(const char* icon, const char* text) {
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 iconSize = ImGui::CalcTextSize(icon);
    ImVec2 textSize = ImGui::CalcTextSize(text);
    return iconSize.x + style.ItemInnerSpacing.x + textSize.x + style.FramePadding.x * 2.0f;
}

static bool IconTextButton(const char* id, const char* icon, const char* text, float width = 0.0f) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (!window || window->SkipItems) {
        return false;
    }

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2 iconSize = ImGui::CalcTextSize(icon);
    ImVec2 textSize = ImGui::CalcTextSize(text);
    float height = ImGui::GetFrameHeight();
    float buttonWidth = width > 0.0f ? width : (iconSize.x + style.ItemInnerSpacing.x + textSize.x + style.FramePadding.x * 2.0f);

    ImVec2 pos = window->DC.CursorPos;
    ImRect bb(pos, ImVec2(pos.x + buttonWidth, pos.y + height));
    ImGui::ItemSize(bb);

    ImGuiID itemId = window->GetID(id);
    if (!ImGui::ItemAdd(bb, itemId)) {
        return false;
    }

    bool hovered = false;
    bool held = false;
    bool pressed = ImGui::ButtonBehavior(bb, itemId, &hovered, &held);
    ImGui::RenderNavHighlight(bb, itemId);
    ImU32 col = ImGui::GetColorU32(held ? ImGuiCol_ButtonActive : (hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button));
    ImGui::RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

    float contentX = bb.Min.x + style.FramePadding.x;
    float textY = bb.Min.y + (bb.GetHeight() - textSize.y) * 0.5f;
    ImGui::RenderText(ImVec2(contentX, textY), icon);
    ImGui::RenderText(ImVec2(contentX + iconSize.x + style.ItemInnerSpacing.x, textY), text);

    return pressed;
}

static bool DrawTitlebarIconButton(ImGuiWindow* window, const char* id, const ImRect& rect, const char* icon, const char* tooltip) {
    if (!window) {
        return false;
    }

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiID itemId = window->GetID(id);
    ImGui::PushClipRect(window->TitleBarRect().Min, window->TitleBarRect().Max, false);
    bool itemAdded = ImGui::ItemAdd(rect, itemId);
    bool hovered = false;
    bool held = false;
    bool pressed = false;
    if (itemAdded) {
        pressed = ImGui::ButtonBehavior(rect, itemId, &hovered, &held);
    }

    (void)style;
    ImVec2 iconSize = ImGui::CalcTextSize(icon);
    float iconX = rect.Min.x + (rect.GetWidth() - iconSize.x) * 0.5f;
    float iconY = rect.Min.y + (rect.GetHeight() - iconSize.y) * 0.5f;
    ImGui::RenderText(ImVec2(iconX, iconY), icon);
    if (hovered && tooltip) {
        ImGui::SetTooltip("%s", tooltip);
    }
    ImGui::PopClipRect();
    return pressed;
}

static bool DrawTitlebarIconTextButton(ImGuiWindow* window, const char* id, const ImRect& rect, const char* icon, const char* text, const char* tooltip) {
    if (!window) {
        return false;
    }

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiID itemId = window->GetID(id);
    ImGui::PushClipRect(window->TitleBarRect().Min, window->TitleBarRect().Max, false);
    bool itemAdded = ImGui::ItemAdd(rect, itemId);
    bool hovered = false;
    bool held = false;
    bool pressed = false;
    if (itemAdded) {
        pressed = ImGui::ButtonBehavior(rect, itemId, &hovered, &held);
    }

    (void)style;
    ImVec2 iconSize = ImGui::CalcTextSize(icon);
    ImVec2 textSize = ImGui::CalcTextSize(text);
    float contentX = rect.Min.x + style.FramePadding.x;
    float textY = rect.Min.y + (rect.GetHeight() - textSize.y) * 0.5f;
    ImGui::RenderText(ImVec2(contentX, textY), icon);
    ImGui::RenderText(ImVec2(contentX + iconSize.x + style.ItemInnerSpacing.x, textY), text);
    if (hovered && tooltip) {
        ImGui::SetTooltip("%s", tooltip);
    }
    ImGui::PopClipRect();
    return pressed;
}

static std::atomic<const ScriptFunction*> g_runningScript{ nullptr };

static void RunScriptAsync(const ScriptFunction* script) {
    if (!script || !script->functionPointer) {
        LogPluginMessage(std::string("RunScriptAsync failed: null function pointer for ")
            + (script && script->name ? script->name : "<null>"), 2);
        return;
    }

    const ScriptFunction* expected = nullptr;
    if (!g_runningScript.compare_exchange_strong(expected, script)) {
        LogPluginMessage("RunScriptAsync skipped: another script is already running.", 1);
        return;
    }

    std::string scriptName = script->name ? script->name : "<null>";
    std::thread([scriptName, fn = script->functionPointer]() {
        LogPluginMessage("Script thread start: " + scriptName);
        try {
            fn();
            LogPluginMessage("Script thread end: " + scriptName);
        }
        catch (...) {
            LogPluginMessage("Script thread exception: " + scriptName, 3);
        }
        g_runningScript.store(nullptr);
    }).detach();
}

extern "C" __declspec(dllexport) const ScriptFunction* GetScripts(size_t* count) {
    return GetRegisteredScripts(count);
}

extern "C" __declspec(dllexport) void LoadDLL() {
    // Host (MemoryError) calls LoadDLL() with no parameters.
    // Keep this light-weight to avoid side effects on inject.
}

extern "C" __declspec(dllexport) void RenderUI() {
    static bool loggedMissingCtx = false;
    ImGuiContext* ctx = GetImGuiContext();
    if (!ctx) {
        if (!loggedMissingCtx) {
            LogPluginMessage("RenderUI: ImGui context not available.", 2);
            loggedMissingCtx = true;
        }
        return;  // Exit if we can't get a valid context
    }
    // Reset the missing-context log once we have a valid context.
    // This avoids spamming while still surfacing future regressions.
    loggedMissingCtx = false;

    ImGui::SetCurrentContext(ctx);
    const ScriptFunction* running = g_runningScript.load();
    std::string windowTitlePrefix = "External Plugin UI";
    std::string windowTitleSuffix;
    if (running && running->name) {
        windowTitleSuffix = " - ";
        windowTitleSuffix += running->name;
    }
    const char* windowTitleId = "###ExternalPluginUI";
    ImGui::SetNextWindowSize(ImVec2(420, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSizeConstraints(ImVec2(440.0f, 200.0f), ImVec2(FLT_MAX, FLT_MAX));
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
    ImGuiStyle& style = ImGui::GetStyle();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(style.FramePadding.x, style.FramePadding.y + 2.0f));
    bool windowVisible = ImGui::Begin(windowTitleId, nullptr, windowFlags);
    ImGui::PopStyleVar();

    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window) {
        ImRect titleBarRect = window->TitleBarRect();
        float padLeft = style.FramePadding.x;
        float padRight = style.FramePadding.x;
        if (!(window->Flags & ImGuiWindowFlags_NoCollapse) && style.WindowMenuButtonPosition == ImGuiDir_Left) {
            padLeft += ImGui::GetFontSize() + style.ItemInnerSpacing.x;
        }
        if (!(window->Flags & ImGuiWindowFlags_NoCollapse) && style.WindowMenuButtonPosition == ImGuiDir_Right) {
            padRight += ImGui::GetFontSize() + style.ItemInnerSpacing.x;
        }

        const float buttonHeight = ImMax(1.0f, titleBarRect.GetHeight());
        const float buttonY = titleBarRect.Min.y + (titleBarRect.GetHeight() - buttonHeight) * 0.5f;
        float leftX = titleBarRect.Min.x + padLeft;
        float rightX = titleBarRect.Max.x - padRight;

        const char* unloadTitleText = "Unload";
        float unloadWidth = CalcIconTextButtonWidth(ICON_FA_POWER_OFF, unloadTitleText);
        ImRect unloadRect(ImVec2(rightX - unloadWidth, buttonY), ImVec2(rightX, buttonY + buttonHeight));
        bool unloadPressed = DrawTitlebarIconTextButton(window, "##unload_title", unloadRect, ICON_FA_POWER_OFF, unloadTitleText, "Unload Plugin");

        float textStartX = leftX;
        float textEndX = unloadRect.Min.x - style.ItemInnerSpacing.x;
        if (textEndX < textStartX) {
            textEndX = textStartX;
        }
        ImRect textRect(ImVec2(textStartX, titleBarRect.Min.y), ImVec2(textEndX, titleBarRect.Max.y));
        ImVec2 prefixSize = ImGui::CalcTextSize(windowTitlePrefix.c_str(), nullptr, true);
        ImVec2 suffixSize = ImGui::CalcTextSize(windowTitleSuffix.c_str(), nullptr, true);

        bool stopPressed = false;
        float cursorX = textRect.Min.x;
        ImGui::PushClipRect(titleBarRect.Min, titleBarRect.Max, false);
        ImGui::RenderTextClipped(textRect.Min, textRect.Max, windowTitlePrefix.c_str(), nullptr, &prefixSize, ImVec2(0.0f, 0.5f));
        cursorX += prefixSize.x;

        if (running) {
            cursorX += style.ItemInnerSpacing.x;
            if (cursorX + buttonHeight <= textRect.Max.x) {
                ImRect runningRect(ImVec2(cursorX, buttonY), ImVec2(cursorX + buttonHeight, buttonY + buttonHeight));
                stopPressed = DrawTitlebarIconButton(window, "##running_title", runningRect, ICON_FA_PERSON_RUNNING, "Stop");
                cursorX = runningRect.Max.x + style.ItemInnerSpacing.x;
            }
        }

        if (!windowTitleSuffix.empty() && cursorX < textRect.Max.x) {
            ImRect suffixRect(ImVec2(cursorX, titleBarRect.Min.y), textRect.Max);
            ImGui::RenderTextClipped(suffixRect.Min, suffixRect.Max, windowTitleSuffix.c_str(), nullptr, &suffixSize, ImVec2(0.0f, 0.5f));
        }
        ImGui::PopClipRect();

        if (stopPressed) {
            LogPluginMessage("Stop requested from titlebar");
            LoopyLoop = false;
        }
        if (unloadPressed) {
            LoopyLoop = false;
            g_unloadRequested = true;
        }
    }

    if (!windowVisible) {
        ImGui::End();
        return;
    }

    static ImGuiTextFilter scriptFilter;
    scriptFilter.Draw("Search");
    ImGui::Separator();

    size_t scriptCount = 0;
    const ScriptFunction* scripts = GetScripts(&scriptCount);
    if (!scripts || scriptCount == 0) {
        ImGui::TextDisabled("No scripts available.");
        if (ImGui::Button("Unload Plugin")) {
            LoopyLoop = false; // Signal scripts to stop if they check this flag
            g_unloadRequested = true;
        }
        ImGui::End();
        return;
    }

    static int selectedIndex = 0;
    if (selectedIndex < 0 || static_cast<size_t>(selectedIndex) >= scriptCount) {
        selectedIndex = 0;
    }

    int firstVisibleIndex = -1;
    bool selectedVisible = false;
    int visibleCount = 0;

    ImGui::BeginChild("left pane", ImVec2(170, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);
    for (size_t i = 0; i < scriptCount; ++i) {
        const char* scriptName = scripts[i].name ? scripts[i].name : "<unnamed>";
        const char* scriptDesc = scripts[i].description ? scripts[i].description : "";
        bool matchesFilter = scriptFilter.PassFilter(scriptName) || scriptFilter.PassFilter(scriptDesc);
        if (!matchesFilter) {
            continue;
        }
        if (firstVisibleIndex < 0) {
            firstVisibleIndex = static_cast<int>(i);
        }
        ++visibleCount;
        bool isRunning = (running == &scripts[i]);
        bool isSelected = (static_cast<size_t>(selectedIndex) == i);
        if (isRunning) {
            ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.1f, 0.6f, 0.1f, 0.6f));
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.1f, 0.7f, 0.1f, 0.7f));
            ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.1f, 0.8f, 0.1f, 0.8f));
        }
        if (ImGui::Selectable(scriptName, isSelected)) {
            selectedIndex = static_cast<int>(i);
        }
        if (isSelected) {
            selectedVisible = true;
        }
        if (isRunning) {
            ImGui::PopStyleColor(3);
        }
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            LogPluginMessage(std::string("Double-click run: ") + scriptName);
            RunScriptAsync(&scripts[i]);
        }
    }
    if (visibleCount == 0) {
        ImGui::TextDisabled("No scripts match search.");
    }
    ImGui::EndChild();

    if (scriptFilter.IsActive() && !selectedVisible && firstVisibleIndex >= 0) {
        selectedIndex = firstVisibleIndex;
    }

    ImGui::SameLine();

    ImGui::BeginGroup();
    ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
    if (visibleCount == 0) {
        ImGui::TextDisabled("No scripts match search.");
    }
    else {
        const char* selectedName = scripts[selectedIndex].name ? scripts[selectedIndex].name : "<unnamed>";
        const char* selectedDesc = scripts[selectedIndex].description ? scripts[selectedIndex].description : "No description available.";
        ImGui::Text("%s", selectedName);
        ImGui::Separator();
        ImGui::TextWrapped("%s", selectedDesc);
    }
    ImGui::EndChild();

    const char* unloadText = "Unload Plugin";
    float unloadWidth = CalcIconTextButtonWidth(ICON_FA_POWER_OFF, unloadText);
    float rowStartX = ImGui::GetCursorPosX();
    float rowY = ImGui::GetCursorPosY();
    float rightEdge = rowStartX + ImGui::GetContentRegionAvail().x;
    float unloadX = rightEdge - unloadWidth;

    if (visibleCount > 0) {
        const char* runIcon = running ? ICON_FA_STOP : ICON_FA_PLAY;
        const char* runText = running ? "Stop" : "Run Selected";
        if (IconTextButton("##run_button", runIcon, runText)) {
            if (running) {
                LogPluginMessage("Stop requested from UI");
                LoopyLoop = false;
            }
            else {
                // Safely handle potential null script name to avoid C6387.
                const char* safeName = scripts[selectedIndex].name ? scripts[selectedIndex].name : "<unnamed>";
                LogPluginMessage(std::string("Run Selected: ") + safeName);
                RunScriptAsync(&scripts[selectedIndex]);
            }
        }
        ImGui::SameLine();
        float minX = ImGui::GetCursorPosX();
        if (unloadX < minX) {
            unloadX = minX;
        }
    }

    ImGui::SetCursorPosX(unloadX);
    ImGui::SetCursorPosY(rowY);
    if (IconTextButton("##unload_button", ICON_FA_POWER_OFF, unloadText)) {
        LoopyLoop = false; // Signal scripts to stop if they check this flag
        g_unloadRequested = true;
    }
    ImGui::EndGroup();

    ImGui::End();
}

extern "C" __declspec(dllexport) void onUnload() {
    LogPluginMessage("onUnload");
    //MessageBoxA(nullptr, "onUnload", "onUnload", MB_OK | MB_ICONERROR);
}

extern "C" __declspec(dllexport) bool UnloadRequested() {
    return g_unloadRequested;
}
