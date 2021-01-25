#include <pigpiod_if2.h>
#include <thread>
#include <mutex>
#include <map>
#include <string>

class Pigpio
{
private:
    int pi;
    int camera_rotate_speed = 0; // -1:下向き, 0:停止, 1:上向き
    map<string,int> motor_states; // <ピン番号,出力>
    motor_states["A1"]=0;
    motor_states["A2"]=0;
    motor_states["B1"]=0;
    motor_states["B2"]=0;
    motor_states["C1"]=0;
    motor_states["C2"]=0;
    motor_states["D1"]=0;
    motor_states["D2"]=0;
    std::mutex camera_rotate_speed_mtx;
    static constexpr int MAX_SERVO_PULSE = 2100;
    static constexpr int MIN_SERVO_PULSE = 900;
    static constexpr int PULSE_INCREMENT = 60;
    //GPIO
    static constexpr int SERVO_PIN = 21; // テキトーにサーボモーター用のピンをGPIO21にしたので好きに変えてください
    // モーターのピン名はMOTOR_[右前から反時計回りにA～D]_[モータードライバのピン1 or 2]
    static constexpr int MOTOR_A_1 = 14;
    static constexpr int MOTOR_A_2 = 15;
    static constexpr int MOTOR_B_1 = 23;
    static constexpr int MOTOR_B_2 = 24;
    static constexpr int MOTOR_C_1 = 17;
    static constexpr int MOTOR_C_2 = 27;
    static constexpr int MOTOR_D_1 = 10;
    static constexpr int MOTOR_D_2 = 9;

    void move_camera_by_polling();
    void move_motor();

public:
    Pigpio();
    ~Pigpio();
    void camera_up();
    void camera_down();
    void camera_stop();
};