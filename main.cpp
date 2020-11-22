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
// シリアル通信部は以下の記事を参考にさせていただきました．
// https://mcommit.hatenadiary.com/entry/2017/07/09/210840

int main(){
    Communication communication;
    std::cout<<"momo'seria-device-name: "<<communication.momo_serial_device_name<<std::endl;
    std::cout<<"my seria-device-name: "<<communication.my_serial_device_name<<std::endl;

    communication.start_stream();

    unsigned char buf[255];
    int fd;                             // ファイルディスクリプタ
    struct termios tio;                 // シリアル通信設定
    int baudRate = B9600;
    int len;
    std::string serial_port = communication.serial_device_dir + communication.my_serial_device_name;

    fd = open(serial_port.c_str(), O_RDWR);
    if (fd < 0) {
        printf("open error\n");
        return -1;
    }

    tio.c_cflag += CREAD;               // 受信有効
    tio.c_cflag += CLOCAL;              // ローカルライン（モデム制御なし）
    tio.c_cflag += CS8;                 // データビット:8bit
    tio.c_cflag += 0;                   // ストップビット:1bit
    tio.c_cflag += 0;                   // パリティ:None

    cfsetispeed( &tio, baudRate );
    cfsetospeed( &tio, baudRate );

    cfmakeraw(&tio);                    // RAWモード
    tcsetattr( fd, TCSANOW, &tio );     // デバイスに設定を行う
    ioctl(fd, TCSETS, &tio);            // ポートの設定を有効にする

    while(1) {
        len = read(fd, buf, sizeof(buf));
        if (0 < len) {
            for(int i = 0; i < len; i++) {
                printf("%c", buf[i]);
            }
            printf("\n");
        }
    }

    close(fd);

    return 0;
}