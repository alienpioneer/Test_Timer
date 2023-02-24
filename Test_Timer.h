#ifndef TEST_TIMER_H
#define TEST_TIMER_H

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include <functional>

class Timer
{	
public:
	Timer(const std::chrono::milliseconds interval) { m_interval = interval; };
	~Timer() {};

	template<class callable, class ...Args>
	void start(callable&& callback, Args&&... args, const bool singleShot = false);
	void stop() { m_continueExec = false; };

private:
	template<class callable, class ...Args>
	void execute(std::function<typename std::result_of<callable(Args...)>::type()> task);

private:
	std::chrono::milliseconds m_interval;
	std::thread m_thread;
	std::atomic<bool> m_singleShot{ false };
	std::atomic<bool> m_continueExec {true };
};

template<class callable, class ...Args>
void Timer::execute(std::function<typename std::result_of<callable(Args...)>::type()> task)
{
	auto start = std::chrono::steady_clock::now();
	
	while (m_continueExec)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		auto elapsedTime = std::chrono::steady_clock::now() - start;
		if (std::duration_cast<std::chrono::milliseconds>(elapsedTime.count() >= m_interval))
		{
			task();
			if (m_singleShot)
				break;
			start = std::chrono::steady_clock::now();
		}
	}
}

template<class callable, class ...Args>
void Timer::start(callable&& callback, Args&&... args, const bool singleShot)
{
	m_singleShot = singleShot;
	std::function<typename std::result_of<callable(Args...)>::type()> task(std::bind(std::forward<callable>(callback), std::forward<Args>(args)...));
	m_thread = std::thread(&this->execute(task), this);
	m_thread.detach();
}

#endif TEST_TIMER_H