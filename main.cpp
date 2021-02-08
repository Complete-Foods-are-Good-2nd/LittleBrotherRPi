#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include "Communication.h"
#include "Pigpio.h"
#include "SoundSystem.h"

int main(int argc, char *argv[]) {
    char buf[255];
    int serial_port;     // ファイルディスクリプタ
    struct termios pts;  // シリアル通信設定
    int baudRate = B9600;
    int len;
    bool isMomoLaunch = true;
    std::string serial_port_path;
    Pigpio pigpio;

    if (argc == 2) {
        serial_port_path = std::string(argv[1]);
        std::cout << "Recevies commands from the serial-port: "
                  << serial_port_path << std::endl;
        isMomoLaunch = false;
    } else if (argc > 2) {
        std::cerr << "Error: Too many arguments, not 2." << std::endl;
        exit(1);
    }

    if (isMomoLaunch) {
        Communication communication;
        std::cout << "momo'seria-device-name: "
                  << communication.momo_serial_device_name << std::endl;
        std::cout << "my seria-device-name: "
                  << communication.my_serial_device_name << std::endl;

        communication.start_stream();

        serial_port_path = communication.serial_device_dir +
                           communication.my_serial_device_name;
    }

    serial_port = open(serial_port_path.c_str(), O_RDWR);
    if (serial_port < 0) {
        std::cerr << "Error: Can not open serial-port: " << serial_port_path
                  << std::endl;
        exit(1);
    }

    pts.c_cflag &= ~PARENB;
    pts.c_cflag &= ~CSTOPB;
    pts.c_cflag &= ~CSIZE;
    pts.c_cflag |= CS8;
    pts.c_cflag &= ~CRTSCTS;
    pts.c_cflag |= CREAD | CLOCAL;

    pts.c_lflag &= ~ICANON;
    pts.c_lflag &= ~ECHO;
    pts.c_lflag &= ~ECHOE;
    pts.c_lflag &= ~ECHONL;
    pts.c_lflag &= ~ISIG;
    pts.c_iflag &= ~(IXON | IXOFF | IXANY);
    pts.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);

    pts.c_oflag &= ~OPOST;
    pts.c_oflag &= ~ONLCR;

    pts.c_cc[VTIME] = 10;
    pts.c_cc[VMIN] = 0;

    cfsetispeed(&pts, baudRate);
    cfsetospeed(&pts, baudRate);

    tcsetattr(serial_port, TCSANOW, &pts);

    while (1) {
        memset(&buf, '\0', sizeof(buf));  //配列を\0で埋めて初期化
        len = read(serial_port, buf, sizeof(buf));
        if (0 < len) {
            //なぜかputsじゃないと大文字が小文字になる
            //設定関係の変数が破壊されてる？
            //とりあえずコマンドをすべて小文字に変えることで対応した
            if (strcmp(buf, "glf") == 0)  // 移動
            {
                // 左前方向に進む
                pigpio.go_lf();
                std::cout << "左前方向に進む" << std::endl;
            } else if (strcmp(buf, "gff") == 0) {
                // 前方向に進む
                pigpio.go_ff();
                std::cout << "前方向に進む" << std::endl;
            } else if (strcmp(buf, "grf") == 0) {
                // 右前方向に進む
                pigpio.go_rf();
                std::cout << "右前方向に進む" << std::endl;
            } else if (strcmp(buf, "grr") == 0) {
                // 右方向に進む
                pigpio.go_rr();
                std::cout << "右方向に進む" << std::endl;
            } else if (strcmp(buf, "grb") == 0) {
                // 右後方向に進む
                pigpio.go_rb();
                std::cout << "右後方向に進む" << std::endl;
            } else if (strcmp(buf, "gbb") == 0) {
                // 後方向に進む
                pigpio.go_bb();
                std::cout << "後方向に進む" << std::endl;
            } else if (strcmp(buf, "glb") == 0) {
                // 左後方向に進む
                pigpio.go_lb();
                std::cout << "左後方向に進む" << std::endl;
            } else if (strcmp(buf, "gll") == 0) {
                // 左方向に進む
                pigpio.go_ll();
                std::cout << "左方向に進む" << std::endl;
            } else if (strcmp(buf, "gsp") == 0) {
                // どの方向であろうと移動を止める
                pigpio.go_stop();
                std::cout << "移動を停止する" << std::endl;
            } else if (strcmp(buf, "cup") == 0)  // カメラの上下
            {
                // カメラを上に動かす
                pigpio.camera_up();
                std::cout << "カメラを上に向ける" << std::endl;
            } else if (strcmp(buf, "cdn") == 0) {
                // カメラを下に動かす
                pigpio.camera_down();
                std::cout << "カメラを下に動かす" << std::endl;
            } else if (strcmp(buf, "csp") == 0) {
                // カメラの上下を停止する
                pigpio.camera_stop();
                std::cout << "カメラの上下を停止する" << std::endl;
            } else if (strcmp(buf, "trt") == 0)  // 旋回
            {
                // 右に旋回する
                pigpio.turn_rt();
                std::cout << "右に旋回する" << std::endl;
            } else if (strcmp(buf, "tlf") == 0) {
                // 左に旋回する
                pigpio.turn_lf();
                std::cout << "左に旋回する" << std::endl;
            } else if (strcmp(buf, "tsp") == 0) {
                // 旋回を停止する
                pigpio.go_stop();
                std::cout << "旋回を停止する" << std::endl;
            } else if (strcmp(buf, "swe") == 0) { // サウンド
                // 爆発音を再生する
                std::cout << "爆発音を再生する" << std::endl;
            } else if (strcmp(buf, "swa") == 0) {
                // 警報音を再生する
                std::cout << "警報音を再生する" << std::endl;
            } else if (strcmp(buf, "lon") == 0) { // ライト
                // ライトを点灯する
                pigpio.light_on();
                std::cout << "ライトを点灯する" << std::endl;
            } else if (strcmp(buf, "lbl") == 0) {
                // ライトを点滅させる
                pigpio.light_blink();
                std::cout << "ライトを点滅させる" << std::endl;
            } else if (strcmp(buf, "lof") == 0) {
                // ライトを消す
                pigpio.light_off();
                std::cout << "ライトを消す" << std::endl;
            } else{
                printf("%s: command not found\n", buf);
            }
        }
    }

    close(serial_port);

    return 0;
}