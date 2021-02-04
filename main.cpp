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
            if (strcmp(buf, "GLF") == 0)  // 移動
            {
                // 左前方向に進む
                pigpio.go_lf();
                puts("左前方向に進む");
            } else if (strcmp(buf, "GFF") == 0) {
                // 前方向に進む
                pigpio.go_ff();
                puts("前方向に進む");
            } else if (strcmp(buf, "GRF") == 0) {
                // 右前方向に進む
                pigpio.go_rf();
                puts("右前方向に進む");
            } else if (strcmp(buf, "GRR") == 0) {
                // 右方向に進む
                pigpio.go_rr();
                puts("右方向に進む");
            } else if (strcmp(buf, "GRB") == 0) {
                // 右後方向に進む
                pigpio.go_rb();
                puts("右後方向に進む");
            } else if (strcmp(buf, "GBB") == 0) {
                // 後方向に進む
                pigpio.go_bb();
                puts("後方向に進む");
            } else if (strcmp(buf, "GLB") == 0) {
                // 左後方向に進む
                pigpio.go_lb();
                puts("左後方向に進む");
            } else if (strcmp(buf, "GLL") == 0) {
                // 左方向に進む
                pigpio.go_ll();
                puts("左方向に進む");
            } else if (strcmp(buf, "GSP") == 0) {
                // どの方向であろうと移動を止める
                pigpio.go_stop();
                puts("移動を停止する");
            } else if (strcmp(buf, "CUP") == 0)  // カメラの上下
            {
                // カメラを上に動かす
                puts("カメラを上に向ける");
                pigpio.camera_up();
            } else if (strcmp(buf, "CDN") == 0) {
                // カメラを下に動かす
                puts("カメラを下に動かす");
                pigpio.camera_down();
            } else if (strcmp(buf, "CSP") == 0) {
                // カメラの上下を停止する
                puts("カメラの上下を停止する");
                pigpio.camera_stop();
            } else if (strcmp(buf, "TRT") == 0)  // 旋回
            {
                // 右に旋回する
                pigpio.turn_rt();
                puts("右に旋回する");
            } else if (strcmp(buf, "TLF") == 0) {
                // 左に旋回する
                pigpio.turn_lf();
                puts("左に旋回する");
            } else if (strcmp(buf, "TSP") == 0) {
                // 旋回を停止する
                pigpio.go_stop();
                puts("旋回を停止する");
            } else {
                printf("%s: command not found\n", buf);
            }
        }
    }

    close(serial_port);

    return 0;
}