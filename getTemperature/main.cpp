#include <iostream>
#include <fstream>          //reading file with ADC values
#include <queue>            //std::queue
#include <iomanip>          //std::setprecision
#include <chrono>           //std::chrono
#include <date.h>           //for time format (UTC)
#include <functional>       //std::function <-- thread/timer
#include <thread>           //for multithreading


#define WAIT_PERIOD_FETCH 10

template <class Precision>
std::string getISOCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();
    return date::format("%FT%TZ", date::floor<Precision>(now));
}

void readFile(void);
void timer_start(std::function<void(void)> func);

std::queue<double> temperQueue;
std::string nowTime = getISOCurrentTimestamp<std::chrono::microseconds>();

int main()
{
    /*starting periodic fuction call to read file with ADC values*/
    timer_start(readFile);


    /*test loop for checking file reading*/
    while(true)
    {

        std::cout << std::setprecision(2) << std::fixed;
        std::cout << temperQueue.front() << std::endl;
        temperQueue.pop();
    }

    return 0;
}


void readFile(void)
{
    std::string str;
    std::ifstream temperStream("temperature.txt");
    auto wakeUpTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(WAIT_PERIOD_FETCH);

    while (std::getline(temperStream, str))
    {
        temperQueue.push(stod(str));
    }
    temperStream.close();
    std::this_thread::sleep_until(wakeUpTime);



}

void timer_start(std::function<void(void)> func){
  std::thread([func](){
    while (true){
      func();
    }
  }).detach();
}
