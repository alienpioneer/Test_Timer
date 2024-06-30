#ifndef TEST_TIMER_H
#define TEST_TIMER_H

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <functional>
#include <type_traits>

class Timer
{	
public:
	Timer(const std::chrono::milliseconds interval, const bool singleShot = false) {
		m_interval = interval; m_singleShot = singleShot;};
	~Timer() {};
	Timer(Timer const& timer) = delete;		// delete copy constructor
	Timer(Timer const&& timer) = delete;	// delete move constructor
	void operator=(Timer const& timer) = delete; // delete copy assignement operator

	// Main function that starts the timer and executes the task after each interval
	// If singleShot is false, the task executes only one, if not it executes periodically
	template<class callable, class... ArgTypes>
	void start(callable&& callback, ArgTypes&&... args)
	{
		std::function<typename std::result_of<callable(ArgTypes...)>::type()> task(std::bind(std::forward<callable>(callback), std::forward<Args>(args)...));

		auto executionFunction = [this, task]() {

			while (m_continueExec)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(m_interval.count()));
				// Task executes, then the timer goes to sleep again for the required interval
				task();
				if (m_singleShot) 
				{ 
					break;
				};
			}
		};

		m_thread = std::thread(std::move(executionFunction));

		m_thread.detach();
	};

	void stop() { m_continueExec = false; };

private:
	std::chrono::milliseconds m_interval;
	std::thread m_thread;
	std::atomic<bool> m_singleShot{ false };
	std::atomic<bool> m_continueExec {true };
};

#endif TEST_TIMER_H