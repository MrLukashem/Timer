
#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include "Timer.h"

void test(int a) {
	std::cout << a << "\n";
}

int main() {

	using namespace std::literals;
	auto t = tmr::makeTimerOnHeap(test, 5);
	t->setReapiting(true);
	t->startTimer(2.5);

	std::this_thread::sleep_for(10s);

	t->stopTimerAndCallCB();

	system("pause");
}