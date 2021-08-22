#ifndef READADC_H_INCLUDED
#define READADC_H_INCLUDED
#endif // READADC_H_INCLUDED

#include <fstream>

#define WAIT_PERIOD_FETCH 100

extern std::queue<double> temperQueue;  //delared in main()

void readFile(void)
{
    std::string str;
    std::ifstream temperStream("temperature.txt");

    if (temperStream.is_open())
    {
        while (std::getline(temperStream, str))
        {
            /*converting adc value to temperature and pushing to queue*/
            temperQueue.push(stod(str));
            auto wakeUpTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(WAIT_PERIOD_FETCH);
            std::this_thread::sleep_until(wakeUpTime);
        }
        temperStream.close();
        //counter++;

    }
    else
    {
        std::cout << "Error opening file";
    }
}



