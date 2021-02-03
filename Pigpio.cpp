#include "Pigpio.h"

Pigpio::Pigpio()
{
    pi = pigpio_start(0, 0);
    motor_states["A1"]=0;
    motor_states["A2"]=0;
    motor_states["B1"]=0;
    motor_states["B2"]=0;
    motor_states["C1"]=0;
    motor_states["C2"]=0;
    motor_states["D1"]=0;
    motor_states["D2"]=0;
    set_mode(pi, SERVO_PIN, PI_OUTPUT);
    set_mode(pi, MOTOR_A_1, PI_OUTPUT);
    set_mode(pi, MOTOR_A_2, PI_OUTPUT);
    set_mode(pi, MOTOR_B_1, PI_OUTPUT);
    set_mode(pi, MOTOR_B_2, PI_OUTPUT);
    set_mode(pi, MOTOR_C_1, PI_OUTPUT);
    set_mode(pi, MOTOR_C_2, PI_OUTPUT);
    set_mode(pi, MOTOR_D_1, PI_OUTPUT);
    set_mode(pi, MOTOR_D_2, PI_OUTPUT);
    std::thread servo_thread(&Pigpio::move_camera_by_polling, this);
    std::thread motor_thread(&Pigpio::move_motor, this);
    servo_thread.detach();
    motor_thread.detach();
}

Pigpio::~Pigpio()
{
    pigpio_stop(pi);
}

void Pigpio::move_camera_by_polling()
{
    int pulse = 1500;
    while (1)
    {
        camera_rotate_speed_mtx.lock();
        pulse += PULSE_INCREMENT * camera_rotate_speed;
        camera_rotate_speed_mtx.unlock();
        if (pulse > MAX_SERVO_PULSE)
            pulse = MAX_SERVO_PULSE;
        else if (pulse < MIN_SERVO_PULSE)
            pulse = MIN_SERVO_PULSE;
        set_servo_pulsewidth(pi, SERVO_PIN, pulse);
        // printf("パルス幅:%d\n", pulse);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Pigpio::move_motor()
{
    while (1)
    {
        gpio_write(pi, MOTOR_A_1, motor_states["A1"]);
        gpio_write(pi, MOTOR_A_2, motor_states["A2"]);
        gpio_write(pi, MOTOR_B_1, motor_states["B1"]);
        gpio_write(pi, MOTOR_B_2, motor_states["B2"]);
        gpio_write(pi, MOTOR_C_1, motor_states["C1"]);
        gpio_write(pi, MOTOR_C_2, motor_states["C2"]);
        gpio_write(pi, MOTOR_D_1, motor_states["D1"]);
        gpio_write(pi, MOTOR_D_2, motor_states["D2"]);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Pigpio::camera_up()
{
    std::lock_guard<std::mutex> lock(camera_rotate_speed_mtx);
    camera_rotate_speed = 1;
}

void Pigpio::camera_down()
{
    std::lock_guard<std::mutex> lock(camera_rotate_speed_mtx);
    camera_rotate_speed = -1;
}

void Pigpio::camera_stop()
{
    std::lock_guard<std::mutex> lock(camera_rotate_speed_mtx);
    camera_rotate_speed = 0;
}

void Pigpio::motor_stop(std::string s)
{
    motor_states[s + "1"] = 1;
    motor_states[s + "2"] = 1;
}

void Pigpio::motor_cw(std::string s)
{
    motor_states[s + "1"] = 1;
    motor_states[s + "2"] = 0;
}

void Pigpio::motor_ccw(std::string s)
{
    motor_states[s + "1"] = 0;
    motor_states[s + "2"] = 1;
}

void Pigpio::go_lf()
{
    motor_ccw("A");
    motor_stop("B");
    motor_cw("C");
    motor_stop("D");
}

void Pigpio::go_ff()
{
    motor_ccw("A");
    motor_cw("B");
    motor_cw("C");
    motor_ccw("D");
}

void Pigpio::go_rf()
{
    motor_stop("A");
    motor_cw("B");
    motor_stop("C");
    motor_ccw("D");
}

void Pigpio::go_rr()
{
    motor_cw("A");
    motor_cw("B");
    motor_ccw("C");
    motor_ccw("D");
}

void Pigpio::go_rb()
{
    motor_cw("A");
    motor_stop("B");
    motor_ccw("C");
    motor_stop("D");
}

void Pigpio::go_bb()
{
    motor_cw("A");
    motor_ccw("B");
    motor_ccw("C");
    motor_cw("D");
}

void Pigpio::go_lb()
{
    motor_stop("A");
    motor_ccw("B");
    motor_stop("C");
    motor_cw("D");
}

void Pigpio::go_ll()
{
    motor_ccw("A");
    motor_ccw("B");
    motor_cw("C");
    motor_cw("D");
}

void Pigpio::turn_rt()
{
    motor_cw("A");
    motor_cw("B");
    motor_cw("C");
    motor_cw("D");
}

void Pigpio::turn_lf()
{
    motor_ccw("A");
    motor_ccw("B");
    motor_ccw("C");
    motor_ccw("D");
}

void Pigpio::go_stop()
{
    motor_stop("A");
    motor_stop("B");
    motor_stop("C");
    motor_stop("D");
}