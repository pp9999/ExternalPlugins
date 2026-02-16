#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "../source/API.h"
#include "ScriptRegistry.h"
using namespace std;

static std::string FormatTimestamp() {
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		now.time_since_epoch()) % 1000;
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm local_tm{};
	localtime_s(&local_tm, &t);

	std::ostringstream oss;
	oss << '[' << std::put_time(&local_tm, "%H:%M:%S") << ':'
		<< std::setw(3) << std::setfill('0') << ms.count() << ']';
	return oss.str();
}

void GE_Example() {
	GrandExchange ge;
	if (ge.IsAtGE()) {
		//if (!ge.IsGEWindowOpen()) { ge.OpenGE(); }
		//ge.PlaceOrder(GrandExchange::ORDER_TYPE::BUY, 1929, "Bucket of water", 12, 10);
		ME::RandomSleep2(800, 1200, 3000);
	}
}

void TestScript() {
	console_text << FormatTimestamp() << " Starting (LoopyLoop=" << std::boolalpha << LoopyLoop
		             << ", Endall=" << Endall << ")" << std::endl;
		DebugImGui.Debugtext_addstream();
		LoopyLoop = true;
	while (LoopyLoop) {
		console_text << FormatTimestamp()
		             << " Running (LoopyLoop=" << std::boolalpha << LoopyLoop
		             << ", Endall=" << Endall << ")" << std::noboolalpha
		             << std::endl;
		DebugImGui.Debugtext_addstream();
		ME::RandomSleep2(400, 0, 0);
	}
}

static ScriptRegistrar g_register_TestScript(
	"TestScript",
	&TestScript,
	"Simple looped test script that logs status while running.");
