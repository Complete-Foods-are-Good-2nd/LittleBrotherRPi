#include <pigpiod_if2.h>
#include <thread>
#include <mutex>

class Pigpio
{
private:
    int pi;
    int camera_rotate_speed = 0; // -1:下向き, 0:停止, 1:上向き
    std::mutex camera_rotate_speed_mtx;
    static constexpr int MAX_SERVO_PULSE = 2100;
    static constexpr int MIN_SERVO_PULSE = 900;
    static constexpr int PULSE_INCREMENT = 60;
    //GPIO
    static constexpr int SERVO_PIN = 21; // テキトーにサーボモーター用のピンをGPIO21にしたので好きに変えてください

    void move_camera_by_polling();

public:
    Pigpio();
    ~Pigpio();
    void camera_up();
    void camera_down();
    void camera_stop();
};