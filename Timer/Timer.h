
#ifndef TIMER_H
#define TIMER_H

#include <functional>
#include <memory>
#include <thread>
#include <chrono>
#include <iostream>

//TODO: comments.
namespace tmr {

	template <typename Function, typename... Args>
	class Timer;

	template <typename Function, typename... Args>
	using TimerSPtr =
		std::shared_ptr<Timer<Function, Args...>>;

	using CallbackFunction = std::function<void(void)>;

	using CallbackPFunction = void(*)(void);

	using DelayFunction = std::function<void(double, CallbackFunction)>;

	using ManagerFunction = std::function<void(void)>;

	using TimerID = unsigned int;

	constexpr TimerID DEFAULT_TIMER_ID = 0;
	constexpr TimerID START_ID_VALUE = 0;
	constexpr double DEFAULT_TIMER_DELAY = 10.0;
	constexpr double MIN_DELAY_VALUE = 1.0;
	constexpr bool DEFAULT_REAPITING_VALUE = false;

	template <typename Function, typename... Args>
	class Timer {

		static TimerID m_free_id;

		TimerID m_id;

		CallbackFunction m_callback;

		std::unique_ptr<std::thread> m_delayThread;

		std::unique_ptr<std::thread> m_managerThread;

		double m_delay;

		DelayFunction m_delayFunction;

		ManagerFunction m_managerFunction;

		bool m_isReapitingTimer;

	public:

		Timer(Function, Args...);

		TimerID getID();

		void startTimer();

		void startTimer(double);

		void stopTimer();

		void stopTimerAndCallCB();

		void setDelay(double);

		void setReapiting(bool);

		CallbackFunction getCallbackFunction();

	private:

		inline void stopThreads() {
			m_managerThread->detach();
			m_delayThread->detach();

			m_delayThread.reset();
			m_managerThread.reset();
		}

		inline void setID() {
			m_id = m_free_id;
			++m_free_id;
		}
	};

	/*
	static private value to store last free id for next Timer instance.
	*/
	template <typename Function, typename... Args>
	TimerID Timer<Function, Args...>::m_free_id = START_ID_VALUE;

	/*
	Default constructor initializating all values and theads controler functions.

	@param1: Any function to use like callback.
	@param2: List of arguments of the function above.
	*/
	template <typename Function, typename... Args>
	Timer<Function, Args...>::Timer(Function func, Args... args) {
		m_callback = std::bind(func, args...);
		m_id = DEFAULT_TIMER_ID;
		m_delay = DEFAULT_TIMER_DELAY;
		m_isReapitingTimer = DEFAULT_REAPITING_VALUE;
		setID();

		m_delayFunction = [&](double delay, CallbackFunction cb) -> void {
			auto first_time_point = std::chrono::steady_clock::now();
			std::chrono::duration<double> diff;
			double count{};

			for (;;) {
				auto second_time_point = std::chrono::steady_clock::now();
				diff = second_time_point - first_time_point;

				count += diff.count();
				if (count >= delay) {
					break;
				}

				first_time_point = second_time_point;
			}

			std::cout << "Wykonanie cb po:" << count << "\n";
			cb();
		};

		m_managerFunction = [&](void) -> void {
			for (;;) {
				m_delayThread =
					std::make_unique<std::thread>(m_delayFunction, m_delay, m_callback);

				m_delayThread->join();
				m_delayThread.reset();

				if (m_isReapitingTimer == false) {
					std::cout << "koncze watek\n";
					break;
				}
			}

			//corectly terminating the thread		
			m_managerThread->detach();
		};
	}

	/*
	Function returns a id of a Timer's instance.

	@return: id.
	*/
	template <typename Function, typename... Args>
	TimerID Timer<Function, Args...>::getID() {
		return m_id;
	}

	/*
	Function returns a given in contructor callback.

	@return: std::function<T> contains a callback.
	*/
	template <typename Function, typename... Args>
	CallbackFunction Timer<Function, Args...>::getCallbackFunction() {
		return m_callback;
	}

	/*
	Create main thread manager.
	*/
	template <typename Function, typename... Args>
	void Timer<Function, Args...>::startTimer() {
		m_managerThread =
			std::make_unique<std::thread>(m_managerFunction);
	}

	/*
	Create main thread manager.

	@param1: A dalay between calls of callback function.
	*/
	template <typename Function, typename... Args>
	void Timer<Function, Args...>::startTimer(double delay) {
		m_delay = delay;

		m_managerThread =
			std::make_unique<std::thread>(m_managerFunction);
	}

	template <typename Function, typename... Args>
	void Timer<Function, Args...>::stopTimer() {
		stopThreads();
	}

	template <typename Function, typename... Args>
	void Timer<Function, Args...>::stopTimerAndCallCB() {
		stopThreads();

		m_callback();
	}

	template <typename Function, typename... Args>
	void Timer<Function, Args...>::setDelay(double delay) {
		if (delay > MIN_DELAY_VALUE) {
			m_delay = delay;
		}
	}

	template <typename Function, typename... Args>
	void Timer<Function, Args...>::setReapiting(bool new_is_reapiting_value) {
		m_isReapitingTimer = new_is_reapiting_value;
	}

	//Timer makers
	template <typename Function, typename... Args>
	Timer<Function, Args...> makeTimerOnStack(Function func, Args... args) {
		return Timer<Function, Args...>(func, args...);
	}

	template <typename Function, typename... Args>
	TimerSPtr<Function, Args...> makeTimerOnHeap(Function func, Args... args) {
		return std::make_shared<Timer<Function, Args...>>(func, args...);
	}

} // namespace tmr
#endif // TIMER_H