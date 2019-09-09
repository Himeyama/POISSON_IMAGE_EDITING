# POISSON_IMAGE_EDITING

# HOW TO INSTALL

```bash
apt install git -y
apt install libopencv-dev
apt install uuid-dev
```

## BUILD

```bash
git clone https://github.com/hikaright/POISSON_IMAGE_EDITING
cd POISSON_IMAGE_EDITING
make
sudo make install
```

# SAMPLE

## COMMAND

```bash
./poisson Yamagata_Aritomo.png yukichi.png -p 115,290 -s 1.55
```

## TARGET
![](https://raw.githubusercontent.com/hikaright/POISSON_IMAGE_EDITING/master/Yamagata_Aritomo.png)

## SOURCE
![](https://raw.githubusercontent.com/hikaright/POISSON_IMAGE_EDITING/master/yukichi.png)

## FUSION
![](https://raw.githubusercontent.com/hikaright/POISSON_IMAGE_EDITING/master/output.png)
