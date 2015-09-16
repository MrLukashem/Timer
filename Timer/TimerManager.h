#pragma once

#ifndef TIMERMANAGER_H
#define TIMERMANAGER

#include <vector>
#include "Timer.h"

namespace tmr {

	constexpr double DEFAULT_TIMER_MANAGER_DELAY = 5.0;

	class TimerManager {

		//meybe a list is better option?
		std::vector<Timer<CallbackFunction>> m_timersVector;

		double m_delay;

	public:

		TimerManager();

		Timer<CallbackFunction>& startTimer(CallbackPFunction);

		Timer<CallbackFunction>& startTimer(CallbackPFunction, double);

		Timer<CallbackFunction>& startTimer(CallbackFunction);

		Timer<CallbackFunction>& startTimer(CallbackFunction, double);

		bool stopTimer(TimerID);

		bool stopTimer(Timer<CallbackPFunction>&);

		bool stopTimer(Timer<CallbackFunction>&);

	private:

		auto findTimerByID(TimerID);

		inline auto getNullItr() {
			return m_timersVector.end();
		}
	};

} //namespace tmr

#endif //TIMERMANAGER_H