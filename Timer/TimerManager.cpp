
#include "TimerManager.h"

using namespace tmr;

TimerManager::TimerManager() {
	m_delay = DEFAULT_TIMER_MANAGER_DELAY;
}

auto TimerManager::findTimerByID(TimerID timer_id) {
	for (auto itr = m_timersVector.begin(); itr != m_timersVector.end(); ++itr) {
		if (itr->getID() == timer_id) {
			return itr;
		}
	}

	return m_timersVector.end();
}

Timer<CallbackFunction>& TimerManager::startTimer(CallbackPFunction cb) {
	auto function = std::function<void(void)>(cb);
	m_timersVector.push_back(makeTimerOnStack(function));

	auto& timer = m_timersVector.back();
	timer.startTimer(m_delay);

	return timer;
}

Timer<CallbackFunction>& TimerManager::startTimer(CallbackPFunction cb, double delay) {
	auto function = std::function<void(void)>(cb);
	m_timersVector.push_back(makeTimerOnStack(function));

	auto& timer = m_timersVector.back();
	timer.startTimer(delay);

	return timer;
}

Timer<CallbackFunction>& TimerManager::startTimer(CallbackFunction cb) {
	m_timersVector.push_back(makeTimerOnStack(cb));
	auto& timer = m_timersVector.back();
	timer.startTimer(m_delay);

	return timer;
}

Timer<CallbackFunction>& TimerManager::startTimer(CallbackFunction cb, double delay) {
	m_timersVector.push_back(makeTimerOnStack(cb));
	auto& timer = m_timersVector.back();
	timer.startTimer(delay);

	return timer;
}

bool TimerManager::stopTimer(TimerID timer_id) {
	auto itr = findTimerByID(timer_id);

	if (itr != getNullItr()) {
		m_timersVector.erase(itr);
		return true;
	}

	return false;
}

bool TimerManager::stopTimer(Timer<CallbackPFunction>& timer) {
	auto itr = findTimerByID(timer.getID());

	if (itr != getNullItr()) {
		m_timersVector.erase(itr);
		return true;
	}

	return false;
}