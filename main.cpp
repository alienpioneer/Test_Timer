#include "Test_Timer.hpp"

void testFn()
{
    std::cout << "Executing task!" << std::endl;
}


int main()
{
	Timer timer(std::chrono::milliseconds(3000));
	timer.start(testFn);

    int previousSecond = 0;

    auto start = std::chrono::steady_clock::now();

    // Test timet -> count 20 seconds then stop the timer
    while (true)
    {
        long long currentTimeDif = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();

        int currentSecond = currentTimeDif / 1000;

        if (currentTimeDif % 1000 == 0U && currentSecond != previousSecond)
        {
            std::cout << currentSecond << std::endl;
            previousSecond = currentSecond;
        }
            
        if (currentSecond >= 20)
        {
            timer.stop();
            break;
        }
    }
    
	std::cout << "End" << std::endl;
	return 0;
}