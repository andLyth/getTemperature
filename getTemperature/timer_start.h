#ifndef TIMER_START_H_INCLUDED
#define TIMER_START_H_INCLUDED

#include <thread>           //for multithreading
#include <functional>       //std::function <-- thread/timer



#endif // TIMER_START_H_INCLUDED

void timer_start(std::function<void(void)> func)
{
  std::thread([func](){
    while (true)
    {
      func();
    }
  }).detach();
}
