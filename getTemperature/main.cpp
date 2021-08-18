#include <iostream>
#include <queue>            //std::queue
#include <precision.h>      //for time precision
#include <timer_Start.h>    //for periodic functioncall
#include <readADC.h>        //for reading ADC values from file
#include <dataHandler.h>    //converting ADC vals to temp and sending to host


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










