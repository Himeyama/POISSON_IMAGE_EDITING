#include <opencv2/opencv.hpp>

#define PX3(image, i, j) image.at<Vec3f>(i, j)
#define PX4(image, i, j) image.at<Vec4f>(i, j)
#define REP(i, n) for (int i = 0; i < n; i++)
#define REP1(i, n) for (int i = 1; i < n; i++)
#define COUNT 8
#define OUTPUT_FILE "output.png"

using namespace cv;

char *cmd_option(int argc, char *argv[], char c) {
  int i;
  for (i = 0; i < argc; i++)
    if (argv[i][0] == '-' && argv[i][1] == c)
      return argv[i + 1];
  return NULL;
}

struct hsv {
  int h, s, v;
};

struct hsv rgb2hsv(int r, int g, int b) {
  int min, max;
  struct hsv hsv;
  r < b &&r < g ? (min = r) : (b < g ? min = b : min = g);
  r > b &&r > g ? (max = r) : (b > g ? max = b : max = g);
  int h = 0;
  int s = 1.0 * (max - min) / max * 255;
  int v = max;
  if (max == r) {
    h = 60.0 * (b - g) / (max - min);
  } else if (max == g) {
    h = 60.0 * (2.0 + (r - b) / (max - min));
  } else if (max == b) {
    h = 60.0 * (4 + (g - r) / (max - min));
  }
  hsv.h = h;
  hsv.s = s;
  hsv.v = v;
  return hsv;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("使用法: %s 元画像ファイル 貼付画像ファイル [オプション]\
        \n貼り付け画像は、3 チャンネルであり、背景は緑色である必要があります\
        \nオプション:\
        \n  -o\t出力ファイル名(デフォルト: %s) \
        \n  -r\t繰り返し回数(デフォルト: %d)
        \n  -p\t貼り付け画像の座標(デフォルト: 0,0)\
        \n  -s\tリサイズ(デフォルト: 1.0)\
        \n",
           argv[0], OUTPUT_FILE, COUNT);
    return -1;
  }

  double size = 1.0;
  int repert = 0;
  if (char *rep = cmd_option(argc, argv, 'r'))
    repert = atoi(rep);

  if (char *siz = cmd_option(argc, argv, 's'))
    size = atof(siz);

  char tmp_p[64] = "0,0";
  char *opt_p = cmd_option(argc, argv, 'p');
  char *p = opt_p ? opt_p : tmp_p;
  int y, x;
  sscanf(p, "%d,%d", &y, &x);
  printf("貼り付け画像の座標: (%d, %d)\n", y, x);

  Mat target, source;
  target = imread(argv[1]);
  printf("読み込み(元画像): %s\n", argv[1]);
  source = imread(argv[2], -1);
  resize(source, source, Size(), size, size);
  printf("読み込み(貼付画像): %s\n", argv[2]);
  printf("貼り付け画像の拡大率: %.1f%%\n", size * 100);

  target.convertTo(target, CV_32FC3);
  source.convertTo(source, CV_32FC4);
  Mat alpha(source.rows, source.cols, CV_8UC1);

  REP(i, source.rows) {
    REP(j, source.cols) {
      int r = PX3(source, i, j)[0];
      int g = PX3(source, i, j)[1];
      int b = PX3(source, i, j)[2];
      struct hsv tmp_c = rgb2hsv(r, g, b);
      if (80 < tmp_c.h && tmp_c.h < 140 && 200 < tmp_c.s && 200 < tmp_c.v) {
        alpha.at<uchar>(i, j) = 0;
      } else {
        alpha.at<uchar>(i, j) = 255;
      }
    }
  }

  REP(k, 2) {
    Mat tmp = alpha.clone();
    REP(i, alpha.rows) {
      REP(j, alpha.cols) {
        if (alpha.at<uchar>(i, j) == 255 &&
            (alpha.at<uchar>(i - 1, j) == 0 || alpha.at<uchar>(i, j + 1) == 0 ||
             alpha.at<uchar>(i + 1, j) == 0 || alpha.at<uchar>(i, j - 1) == 0 ||
             i == 1 || j == 1 || i == alpha.rows - 1 || j == alpha.cols - 1)) {
          tmp.at<uchar>(i, j) = 0;
        }
      }
    }
    alpha = tmp;
  }

  // ポアソンフィルター
  REP(k, (repert ? repert : COUNT)) {
    printf("\e[G%d回目のループ...", k + 1);
    REP1(i, source.rows) {
      REP1(j, source.cols) {
        if (alpha.at<uchar>(i, j) == 255) {
          REP(rgb, 3) {
            PX3(target, i + y, j + x)
            [rgb] = (PX3(target, i - 1 + y, j + x)[rgb] -
                     PX3(source, i - 1, j)[rgb] +
                     PX3(target, i + y, j + 1 + x)[rgb] -
                     PX3(source, i, j + 1)[rgb] +
                     PX3(target, i + 1 + y, j + x)[rgb] -
                     PX3(source, i + 1, j)[rgb] +
                     PX3(target, i + y, j - 1 + x)[rgb] -
                     PX3(source, i, j - 1)[rgb]) /
                        4 +
                    PX3(source, i, j)[rgb];
          }
          // REP(rgb, 3) PX3(target, i+y, j+x)[rgb] = alpha.at<uchar>(i,
          // j)/*PX3(alpha, i, j)[rgb]*/;
        }
      }
    }
  }
  printf("\b\b\b   \n");
  target.convertTo(target, CV_8UC3);

  char *output_fn = cmd_option(argc, argv, 'o');
  if (output_fn) {
    imwrite(output_fn, target);
    printf("書き込み: %s\n", output_fn);
  } else {
    char output_file[] = OUTPUT_FILE;
    imwrite(output_file, target);
    printf("書き込み: %s\n", output_file);
  }
  imwrite("tmp_alpha.png", alpha);

  return 0;
}
