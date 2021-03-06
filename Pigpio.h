#include <pigpiod_if2.h>
#include <map>
#include <mutex>
#include <string>
#include <thread>

class Pigpio {
   private:
    int pi;
    int camera_rotate_speed = 0;  // -1:下向き, 0:停止, 1:上向き
    std::mutex camera_rotate_speed_mtx;
    static constexpr int MAX_SERVO_PULSE = 2100;
    static constexpr int MIN_SERVO_PULSE = 900;
    static constexpr int PULSE_INCREMENT = 60;
    int light_a=0;
    int light_b=0;
    std::mutex light_mtx;
    // GPIO
    static constexpr int SERVO_PIN =
        21;  // テキトーにサーボモーター用のピンをGPIO21にしたので好きに変えてください
    // モーターのピン名はMOTOR_[右前から反時計回りにA～D]_[モータードライバのピン1
    // or 2]
    static constexpr int MOTOR_A_1 = 14;
    static constexpr int MOTOR_A_2 = 15;
    static constexpr int MOTOR_B_1 = 23;
    static constexpr int MOTOR_B_2 = 24;
    static constexpr int MOTOR_C_1 = 17;
    static constexpr int MOTOR_C_2 = 27;
    static constexpr int MOTOR_D_1 = 10;
    static constexpr int MOTOR_D_2 = 9;
    // ライト
    static constexpr int LIGHT_PIN = 16;

    void move_camera_by_polling();
    void apply_move_motor();
    void motor_stop(std::string s);
    void motor_cw(std::string s);
    void motor_ccw(std::string s);
    void set_light();

   public:
    Pigpio();
    ~Pigpio();
    std::map<std::string, int> motor_states;  // <ピン番号,出力>
    void camera_up();
    void camera_down();
    void camera_stop();
    void go_lf();
    void go_ff();
    void go_rf();
    void go_rr();
    void go_rb();
    void go_bb();
    void go_lb();
    void go_ll();
    void turn_rt();
    void turn_lf();
    void go_stop();
    void light_off();
    void light_on();
    void light_blink();
};