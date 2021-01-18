#include <pigpiod_if2.h>
#include <thread>
#include <mutex>

constexpr int cexpr_test = 1;

class Pigpio
{
private:
    int pi;
    int camera_rotate_speed = 0; // -1:下向き, 0:停止, 1:上向き
    std::mutex camera_rotate_speed_mtx;
    
    static constexpr int MAX_SERVO_PULSE = 2100;
    static constexpr int MIN_SERVO_PULSE = 900;
    static constexpr int PULSE_INCREMENT = 60;

    static constexpr int SERVO_PIN = 21;

public:
    Pigpio();
    ~Pigpio();
    void rotate_camera_servo();
    void camera_up();
    void camera_down();
    void camera_stop();
};