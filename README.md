# LittleBrotherRPi
LIttle BrotherのRaspberry Pi用ソフトウェアです．
## 環境構築
以下をインストールしておいてください．
- socat
- [WebRTC Native Client Momo](https://github.com/shiguredo/momo)
- Boost C++ Libraries

## ビルド
以下で実行ファイルmainがビルドされます．
```
make
```

## 動かす
環境変数を設定してください．
```
export LB_SERIAL_DEVICE_DIR=
export LB_SOCAT_DIR=
export LB_AYAME_SIGNALINGKEY=
export LB_AYAME_ROOM_ID=
export LB_MOMO_DIR=
```
実行
```
./main
```

## オプション
Usage: [-p serial-port] [-h]  
[-p serial-port] momoを起動せず，serial-portからコマンドを受け取ります．  
[-h] 現在のhelpを表示します．

## コマンド対応表
https://gist.github.com/nPeeech/5d9be64e710d96d02621656b444bbe93