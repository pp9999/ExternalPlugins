#include <iostream>
#include "../source/API.h"
using namespace std;

void GE_Example() {
	GrandExchange ge;
	if (ge.IsAtGE()) {
		if (!ge.IsGEWindowOpen()) { ge.OpenGE(); }
		ge.PlaceOrder(GrandExchange::ORDER_TYPE::BUY, 1929, "Bucket of water", 12, 10);
		ME::RandomSleep2(800, 1200, 3000);
	}
}

void TestScript() {
	while (LoopyLoop && !Endall) {
		cout << "Heloooo" << endl;
	}
}