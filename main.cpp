#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include "Communication.h"

int main(int argc, char* argv[]){
    unsigned char buf[255];
    int serial_port;                             // ファイルディスクリプタ
    struct termios pts;                 // シリアル通信設定
    int baudRate = B9600;
    int len;
    int i, opt;
    opterr = 0; //getopt()のエラーメッセージを無効にする
    bool isMomoLaunch = true;
    std::string serial_port_path;

    while((opt = getopt(argc, argv, "hp:")) != -1){
        switch (opt)
        {
        case 'p':
            printf("-pが渡されました．momoを起動せず，シリアルポート:%s からコマンドを受け取ります．\n", optarg);
            isMomoLaunch = false;
            serial_port_path = std::string(optarg);
            break;

        case 'h':
            std::cout<<"Usage: [-p serial-port] [-h]"<<std::endl;
            std::cout<<"[-p serial-port] momoを起動せず，serial-portからコマンドを受け取ります．"<<std::endl;
            std::cout<<"[-h] 現在のhelpを表示します．"<<std::endl;
            return 0;

            break;
        
        default:
            break;
        }
    }
    
    if(isMomoLaunch){
        Communication communication;
        std::cout<<"momo'seria-device-name: "<<communication.momo_serial_device_name<<std::endl;
        std::cout<<"my seria-device-name: "<<communication.my_serial_device_name<<std::endl;

        communication.start_stream();

        serial_port_path = communication.serial_device_dir + communication.my_serial_device_name;
    }

    serial_port = open(serial_port_path.c_str(), O_RDWR);
    if (serial_port < 0) {
        printf("can not open file\n");
        exit(1);
    }

    pts.c_cflag &= ~PARENB;
    pts.c_cflag &= ~CSTOPB;
    pts.c_cflag &= ~CSIZE;
    pts.c_cflag |= CS8;
    pts.c_cflag &= ~CRTSCTS;
    pts.c_cflag |= CREAD | CLOCAL;


    pts.c_cflag += CLOCAL;              // ローカルライン（モデム制御なし）
    pts.c_cflag += CS8;                 // データビット:8bit
    pts.c_cflag += 0;                   // ストップビット:1bit
    pts.c_cflag += 0;                   // パリティ:None

    cfsetispeed( &pts, baudRate );
    cfsetospeed( &pts, baudRate );

    cfmakeraw(&pts);                    // RAWモード
    tcsetattr( serial_port, TCSANOW, &pts );     // デバイスに設定を行う
    ioctl(serial_port, TCSETS, &pts);            // ポートの設定を有効にする

    while(1) {
        len = read(serial_port, buf, sizeof(buf));
        if (0 < len) {
            for(int i = 0; i < len; i++) {
                printf("%c", buf[i]);
            }
            printf("\n");
        }
    }

    close(serial_port);

    return 0;
}