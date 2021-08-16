#include <iostream>
#include <fstream>          //reading file with ADC values
#include <queue>            //std::queue
#include <iomanip>          //std::setprecision


std::string readFile(void);

std::queue<double> temperQueue;

int main()
{
    double test = 0.0;
    while(true)
    {
        test = temperQueue.front();
        temperQueue.pop();
        std::cout << std::setprecision(2) << std::fixed;
        std::cout << test << std::endl;
    }

    return 0;
}


std::string readFile(void)
{
    std::string str;

    try
    {
        std::ifstream temperStream("temperature.txt");
        while (std::getline(temperStream, str))
        {
            temperQueue.push (stod(str));
        }
    }
    catch (const std::ifstream::failure& e)
    {
        std::cout << "Exception opening/reading file" << std::endl;
    }





}
