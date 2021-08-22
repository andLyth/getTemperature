#ifndef DATAHANDLER_H_INCLUDED
#define DATAHANDLER_H_INCLUDED
#endif // DATAHANDLER_H_INCLUDED

#include <json.hpp>
#include <windows.h>
#include <deque>

#define LOCAL_ADDRESS "http://localhost:5000/api/temperature";
#define LOCAL_BACKUP_ADDRESS "http://localhost:5000/api/temperature/missing";

#define WAIT_PERIOD_SEND 1000               /*wakeup period for periodic function call*/
#define SLOPE 23.0/952.0d                   /*linear function for ADC (x) -> temp conversion: f(x) = (23/952)*x - 50 */
#define INTERCEPT 50.0d                     /*linear function for ADC (x) -> temp conversion: f(x) = (23/952)*x - 50 */
#define MAX_VALUE_BACKUP_ELEMENT_SIZE 10;   /*number of max values to be backed up for error handling (errorcode: 500)*/




inline void createOutputs(double* ptr_maxVal, double* ptr_minVal, double* ptr_average,
                          auto* ptr_endTime, auto* ptr_startTime);
inline void print(std::queue<double> maxValuesBackup);



extern std::queue<double> temperQueue;      //delared in main()
extern std::string str;                     //declared in readADC.




void printData(void)
{
    auto wakeUpTime = std::chrono::steady_clock::now() + std::chrono::milliseconds(WAIT_PERIOD_SEND);
    double temperature = 0.0;
    double maxVal = 0.0;
    double minVal = 50.0;
    double sum = 0.0;
    double average = 0.0;
    int countrForDivision = 0;

    static std::string endTime = "end time not generated";
    static std::string startTime; //empty on first run
    static std::deque<nlohmann::json> backupDeque;

    if(startTime.empty()) startTime = getISOCurrentTimestamp<std::chrono::nanoseconds>();

    while (!temperQueue.empty())
    {
        //convert ADC value to temperature and arrange temp value accordingly
        temperature = SLOPE * temperQueue.front()- INTERCEPT;
        temperQueue.pop();
        if(temperature > maxVal) maxVal = temperature;
        else if(temperature < minVal) minVal = temperature;
        sum += temperature;
        countrForDivision++;
    }

    //calculate average for timeblock
    average = sum/countrForDivision;

    //generate end time for this periods temperature readings.
    endTime = getISOCurrentTimestamp<std::chrono::nanoseconds>();

    //create output files
    createOutputs(&maxVal, &minVal, &average, &endTime, &startTime);

    std::this_thread::sleep_until(wakeUpTime);
}


void createOutputs(double* ptr_maxVal, double* ptr_minVal, double* ptr_average,
                   auto* ptr_endTime, auto* ptr_startTime)
{
    static std::deque<nlohmann::json> backupDeque;
    std::ofstream o1("temeratureCalculations.json");
    std::ofstream o2("backup.json");

    //create json object
    nlohmann::json j = {
        {"time: ",
            {"start: ", *ptr_startTime},
            {"end: ", *ptr_endTime}
        },
        {"min: ", *ptr_minVal},
        {"max: ", *ptr_maxVal},
        {"average: ", *ptr_average},
    };



    //create JSON file for sending. Will be overwritten each WAIT_PERIOD_SEND
    std::cout << std::setprecision(2);
    o1 << std::setw(3) << j << std::endl;
    o1.close();

    //backup ten last JSON objects into JSON backup file. . Will be overwritten each WAIT_PERIOD_SEND
    if(backupDeque.size() >= MAX_VALUE_BACKUP_ELEMENT_SIZE) backupDeque.pop_front();
    backupDeque.push_back(j);
    for (std::deque<nlohmann::json>::iterator it = backupDeque.begin(); it!=backupDeque.end(); ++it)
    {
        std::cout << std::setprecision(2);
        o2 << std::setw(3) << j << std::endl;
    }
    o2.close();

    //test by printing
    //print(maxValuesBackup);

    //generate start time for next period.
    *ptr_startTime = getISOCurrentTimestamp<std::chrono::nanoseconds>();

}


//Pseudo function for filesending to localhost.
void sendFile(void)
{

    //pseudo code

    //send ."temeratureCalculations.json" or json object (j) to LOCAL_ADDRESS

    //wait for reply (code)

    //if code is 500, send "backup.json" to LOCAL_BACKUP_ADDRESS
}


//function for testing;
void print(std::queue<double> maxValuesBackup)
{
    //print json object
    std::ifstream i("temeratureCalculations.json");
    nlohmann::json j;
    i>>j;
    for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it)
    {
        std::cout << std::setprecision(2);
        std::cout << *it <<std::endl;
    }

    //print backup values
        while(!maxValuesBackup.empty())
    {
        std::cout << maxValuesBackup.front() << std::endl;
        maxValuesBackup.pop();

    }

}



