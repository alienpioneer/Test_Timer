#include "Test_Timer.h"

void testFn()
{
    std::cout << "Executing !" << std::endl;
}


int main()
{
	Timer timer(std::chrono::milliseconds(3000));
	timer.start(testFn);

    auto start = std::chrono::steady_clock::now();
    while (true)
    {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() > 20000)
            timer.stop();
    }
    
	std::cout << "End" << std::endl;
	return 0;
}