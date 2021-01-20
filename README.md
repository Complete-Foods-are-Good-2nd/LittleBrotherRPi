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

## Momoを起動せずに，コマンドのテストだけを目的に動かす
複数のコマンドが動き続けるので，ターミナルを複数用意するか，tmuxなどで分割してください．

シリアル通信に使うためにpyserialを入れる  ．
```
$ sudo apt update
$ sudo apt upgrade
$ sudo apt install python3-pip
$ pip3 install pyserial
```
ループしてる仮想シリアルポートを作る．
```
$ socat -d -d pty,raw,echo=0 pty,raw,echo=0
```
例えば以下のように出力された場合は/dev/pts/3と/dev/pts/4が繋がっている．これらの名前は次で使うのでメモしておく．
```
2021/01/20 07:06:41 socat[5080] N PTY is /dev/pts/3
2021/01/20 07:06:41 socat[5080] N PTY is /dev/pts/4
2021/01/20 07:06:41 socat[5080] N starting data transfer loop with FDs [5,5] and [7,7]
```
LittleBrotherRPiを実行する．
```
$  ./main /dev/pts/3 //先ほど作った仮想シリアルポートの片方を渡す
```

コマンドを送る．
```
$ python3 ./command_transmitter.py /dev/pts/4 //仮想シリアルポートのLittleBrotherRPiに渡していないほうを渡す
```
`>> `というプロンプトが出てくるので，コマンドを入力すればLittleBrotherRPiが受け取り，何かしら出力してくれるはず...

## コマンド対応表
https://gist.github.com/nPeeech/5d9be64e710d96d02621656b444bbe93