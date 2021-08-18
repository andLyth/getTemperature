#include <iostream>
#include <fstream>          //reading file with ADC values
#include <queue>            //std::queue
#include <precision.h>      //for time precision
#include <timer_Start.h>


#define WAIT_PERIOD_FETCH 2000
#define WAIT_PERIOD_SEND 2000
#define SLOPE 23.0/952.0d
#define INTERCEPT 50.0d


void readFile(void);
void printData(void);
void timer_start(std::function<void(void)> func);
double convertToTemp(double adcValue);

std::queue<double> temperQueue;
std::string nowTime = getISOCurrentTimestamp<std::chrono::microseconds>();



int main()
{
    /*starting periodic function call to read file with ADC values*/
    timer_start(readFile);
    timer_start(printData);


    /*to keep threads alive*/
    while(true)
    {


    }

    return 0;
}


void readFile(void)
{
    std::string str;
    std::ifstream temperStream("temperature.txt");
    auto wakeUpTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(WAIT_PERIOD_FETCH);

    if (temperStream.is_open())
    {
        while (std::getline(temperStream, str))
        {
            temperQueue.push(convertToTemp(stod(str)));
        }
        temperStream.close();
        std::this_thread::sleep_until(wakeUpTime);
    }
    else
    {
        std::cout << "Error opening file";
    }
}

/*function for code testing*/
void printData(void)
{
    auto wakeUpTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(WAIT_PERIOD_SEND);
    while (!temperQueue.empty())
    {
        std::cout << std::setprecision(2) << std::fixed;
        std::cout << temperQueue.front() << std::endl;
        temperQueue.pop();

    std::this_thread::sleep_until(wakeUpTime);
    }
}


/*linear function for adc -> temp conversion: f(x) = (23/952)*x - 50 */
double convertToTemp(double adcValue)
{
    return SLOPE * adcValue - INTERCEPT;
}



