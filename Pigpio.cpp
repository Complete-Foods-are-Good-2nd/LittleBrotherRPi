#include "Pigpio.h"

Pigpio::Pigpio()
{
    pi = pigpio_start(0, 0);
    set_mode(pi, SERVO_PIN, PI_OUTPUT);
    std::thread servo_thread(&Pigpio::move_camera_by_polling, this);
    servo_thread.detach();
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