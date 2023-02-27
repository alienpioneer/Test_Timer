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

	template<class callable, class... ArgTypes>
	void start(callable&& callback, ArgTypes&&... args)
	{
		std::function<typename std::result_of<callable(ArgTypes...)>::type()> task(std::bind(std::forward<callable>(callback), std::forward<Args>(args)...));

		//task();

		auto exec = [this, task]() {

			auto start = std::chrono::steady_clock::now();

			while (m_continueExec)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
				auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start);
				//std::cout << elapsedTime.count() << ";";
				if (elapsedTime.count() >= m_interval.count())
				{
					task();
					if (m_singleShot) { break; };
					start = std::chrono::steady_clock::now();
				}
			}
		};

		m_thread = std::thread(std::move(exec));

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