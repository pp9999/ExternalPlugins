#include "API.h"
#include <filesystem>
#include <iostream>


static const int MAX_IDLE_TIME_MINUTES = 5;

static time_t startTime = std::time(nullptr);
static time_t afk = std::time(nullptr);
static int currentCount;
static int totalGained;

//18204 chronicle fragment, other peopls 18205
//19884 guthix butterfly       	
//26022 seren spirit
//27228 Divine blessing
//27297 Forge phoenix
//28411 catalyst
//30599 h Pumpkin 
//15451 Fire spirit
//find random events, and click them
static bool RandomEvents() {
	std::vector <AllObject> F_obj = ME::GetAllObjArrayInteract({ 19884, 26022, 27228, 27297, 28411, 30599, 15451, 18204 }, 20, { 1 });
	if (!F_obj.empty()) {
		std::cout << "Random event object detected: trying to click" << std::endl;
		//if(ME::Math_AO_ValueEqualsArr({18204,18205,19884,26022,27228,27297,28411,30599 }, F_obj)) {//if seperation is needed
		if (DO::DoAction_NPC__Direct(0x29, OFF_ACT::InteractNPC_route, F_obj.front())) {
			ME::RandomSleep2(1500, 4050, 12000);
			return true;
		}
	}
	return false;
}

static void idleCheck() {
	time_t currentTime = std::time(nullptr);
	time_t timeDiff = currentTime - afk;
	time_t randomTime = rand() % (static_cast<int>((MAX_IDLE_TIME_MINUTES * 60) * 0.3)) + (MAX_IDLE_TIME_MINUTES * 60 * 0.6);

	if (timeDiff > randomTime) {
		ME::PIdle2();
		afk = currentTime;
	}
}

static std::string getTime() {
	//RunTime
	std::time_t currentTime = std::time(nullptr);

	//run time in seconds
	int runTime = static_cast<int>(std::difftime(currentTime, startTime));

	//hh:mm:ss format
	int hours = runTime / 3600;
	int minutes = (runTime % 3600) / 60;
	int seconds = runTime % 60;

	//run time as string
	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << hours << ":"
		<< std::setfill('0') << std::setw(2) << minutes << ":"
		<< std::setfill('0') << std::setw(2) << seconds;
	std::string timeStr = oss.str();
	return timeStr;
}


static int getGainedCount(int start, int current) {
	return current - start;
}
//UI Example
void  onDraw() {
	ImGui::Begin("AutoDivination", 0);
	ImGui::Text("Timer %s", getTime());
	if (ImGui::BeginChild("Artistic", ImVec2(800, 500), true)) {
		ImGui::Text("     ^     ");
		ImGui::Text("    ^^^    ");
		ImGui::Text("   ^^^^^   ");
		ImGui::Text("  ^^^^^^^  ");
		ImGui::Text(" ^^^^^^^^^ ");
		ImGui::Text("^^^^^^^^^^^");
		ImGui::Text("^^^^^^^^^^^^");
		ImGui::Text("     ||     ");
	}

	ImGui::End();
}


void AutoDivination() {

	static int startCount = ME::InvItemcountStack_String("energy");
	vector<AllObject>EnergyRiftOpen;
	vector<AllObject>EnergyRift;
	vector<AllObject>EnrichedSpring;
	vector<AllObject>RegularSpring;
	DrawImGui(onDraw);

	while (LoopyLoop && !Endall) {
		idleCheck();
		RandomEvents();
		currentCount = ME::InvItemcountStack_String("energy");
		totalGained = getGainedCount(startCount, currentCount);
		ScripCuRunning2 = "Energy Gained: ";
		ScripCuRunning2.append(to_string(totalGained));
		ScripCuRunning1 = "Waiting";


		if (ME::InvFull_()) {
			ScripCuRunning1 = "Looking to convert";
			EnergyRiftOpen = ME::GetAllObjArrayInteract({ 93489 }, 40, { 0 });
			if (!EnergyRiftOpen.empty()) {
				Sleep(300 + rand() % 300);
				DO::DoAction_Object_Direct(0xc8, OFF_ACT::GeneralObject_route0, EnergyRiftOpen.front());
				Sleep(300 + rand() % 300);
			}
			else {
				EnergyRift = ME::GetAllObjArrayInteract({ 87306 }, 40, { 12 });
				if (!EnergyRift.empty()) {
					Sleep(300 + rand() % 300);
					DO::DoAction_Object_Direct(0xc8, OFF_ACT::GeneralObject_route0, EnergyRift.front());
					Sleep(300 + rand() % 300);
				}
			}
			Sleep(500 + rand() % 500);
		}
		else {
			if (!ME::CheckAnim(100) && !ME::ReadPlayerMovin()) {
				ScripCuRunning1 = "Catching energy";
				EnrichedSpring = ME::GetAllObjArrayInteract_str({ "Enriched" }, 40, { 1 });
				RegularSpring = ME::GetAllObjArrayInteract_str({ "spring","wisp" }, 40, { 1 });
				std::cout <<"Reg" << RegularSpring.size() << endl;
				std::cout <<"Enriched" << EnrichedSpring.size() << endl;
				if (!EnrichedSpring.empty()) {
					Sleep(300 + rand() % 300);
					DO::DoAction_NPC__Direct(0xc8, OFF_ACT::InteractNPC_route, EnrichedSpring.front());
					Sleep(300 + rand() % 300);
				}
				else {
					RegularSpring = ME::GetAllObjArrayInteract_str({ "spring","wisp" }, 40, { 1 });
					std::cout << RegularSpring.size() << endl;
					if (!RegularSpring.empty()) {
						Sleep(300 + rand() % 300);
						DO::DoAction_NPC__Direct(0xc8, OFF_ACT::InteractNPC_route, RegularSpring.front());
						Sleep(300 + rand() % 300);
					}
				}
			}
		}
		Sleep(1000);
	}
}