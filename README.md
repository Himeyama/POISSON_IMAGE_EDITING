# POISSON_IMAGE_EDITING

POISSON IMAGE EDITING はポアソン方程式の連立方程式を解き、シームレスな画像合成の方法です。

# インストール方法

Ubuntuのみ対応

## 依存するパッケージ

```bash
sudo apt install git -y
sudo apt install libopencv-dev
sudo apt install uuid-dev
```

## ビルド

```bash
git clone https://github.com/hikaright/POISSON_IMAGE_EDITING
cd POISSON_IMAGE_EDITING
make
sudo make install
```

## 問題
> PKG_CONFIG_PATH が設定されていない場合
```bash
PKG_CONFIG_PATH=/usr/lib/x86_64-linux-gnu/pkgconfig make

```
