#include "Pigpio.h"

Pigpio::Pigpio(){
    pi = pigpio_start(0, 0);
}

Pigpio::~Pigpio(){
    pigpio_stop(pi);
}