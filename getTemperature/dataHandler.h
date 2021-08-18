#ifndef DATAHANDLER_H_INCLUDED
#define DATAHANDLER_H_INCLUDED

#include<windows.h>

#define WAIT_PERIOD_SEND 2000
#define SLOPE 23.0/952.0d   /*linear function for adc (x) -> temp conversion: f(x) = (23/952)*x - 50 */
#define INTERCEPT 50.0d     /*linear function for adc (x) -> temp conversion: f(x) = (23/952)*x - 50 */

#endif // DATAHANDLER_H_INCLUDED



extern std::queue<double> temperQueue;      //delared in main()
extern std::string str;                     //declared in readADC.
std::queue<double> randQ;


void printData(void)
{
    auto wakeUpTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(WAIT_PERIOD_SEND);
    double temperature = 0.0;
    double maxVal = 0.0;
    double minVal = 50.0;
    double sum = 0.0;
    int counter = 0;

    while (!temperQueue.empty())
    {


        temperature = SLOPE * temperQueue.front()- INTERCEPT;
        //std::cout << "temp; "<<temperature << std::endl;
        if(temperature > maxVal) maxVal = temperature;
        else if(temperature < minVal) minVal = temperature;
        sum += temperature;
        //std::cout << "sum; "<<sum << std::endl;
        counter++;
        //std::cout << "count; "<<counter << std::endl;

        temperQueue.pop();

    }

    std::cout << std::setprecision(2) << std::fixed;
    std::cout << "Max: " << maxVal << std::endl;
    std::cout << "Min: " << minVal << std::endl;
    std::cout <<  "Average: " << sum/counter << std::endl;
    std::cout <<  "counter: " << counter << std::endl;

    temperature = 0.0;
    maxVal = 0.0;
    minVal = 50.0;
    sum = 0.0;
    counter = 0;

    std::cout <<  std::endl;
    std::cout <<  "Nuller ut: " << std::endl;
    std::cout << std::setprecision(2) << std::fixed;
    std::cout << "Max: " << maxVal << std::endl;
    std::cout << "Min: " << minVal << std::endl;
    std::cout <<  "Average: " << sum/counter << std::endl;
    std::cout <<  "counter: " << counter << std::endl;
    std::cout <<  std::endl;
    std::cout <<  std::endl;
    std::cout <<  std::endl;


    //Sleep(3000);

    std::this_thread::sleep_until(wakeUpTime);
}


