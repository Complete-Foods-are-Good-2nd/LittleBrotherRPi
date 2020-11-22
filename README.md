# LittleBrotherRPi
LIttle BrotherのRaspberry Pi用ソフトウェアです．
## 環境構築
以下をインストールしておいてください．
- socat
- [WebRTC Native Client Momo](https://github.com/shiguredo/momo)

## ビルド
以下で実行ファイルmainがビルドされます．
```
make
```

## 動かす
環境変数を設定してください．
```
export LB_SERIAL_DEVICE_DIR=
export LB_SOCAT_DIR
export LB_AYAME_SIGNALING
export LB_AYAME_ROOM_ID
export LB_MOMO_DIR
```
実行
```
./main
```
