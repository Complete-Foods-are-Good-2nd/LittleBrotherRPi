#include <pigpiod_if2.h>
#include <thread>
#include <mutex>

class Pigpio
{
private:
    int pi;
public:
    Pigpio();
    ~Pigpio();    
};