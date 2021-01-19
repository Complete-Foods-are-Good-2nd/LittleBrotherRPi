# LittleBrotherRPi
LIttle BrotherのRaspberry Pi用ソフトウェアです．
## 環境構築
以下をインストールしておいてください．
- socat
    ```
    sudo apt install socat
    ```
- [WebRTC Native Client Momo](https://github.com/shiguredo/momo)
- Boost C++ Libraries
    ```
    sudo apt install libboost-dev
    ```

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
./main //Momoを自動で起動する
./main <シリアルポート> //Momoを起動せず，指定されたシリアルポートからコマンドを受け取る
```

## コマンド対応表
https://gist.github.com/nPeeech/5d9be64e710d96d02621656b444bbe93