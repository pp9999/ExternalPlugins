#pragma once
#include <functional>
#include "API.h"

void DrawImGui(std::function<void()> drawFunction) {
	ImGuiContext* mainContext = GetImGuiContext();
	ImGui::SetCurrentContext(mainContext);
	SubscribeToRenderEvent(drawFunction);
}
