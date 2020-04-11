//**************************
// Mandelbrot Fractal
//**************************
#define TEST  0
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Resolution of image
const int W = 1920;
const int H = 1080;

// Maximal color
const int alpha = 255;
// Maximal iteration
const int max_iteration = 10000;
// zoom factor
const long double s = 0.97;

long double ss;
long double kappa;
int sigma;

// Position of point we are zooming in
// long double X = -0.789374599271466936740382412558;
// long double Y = 0.163089252677526719026415054868;
long double X = -1.477110786384222313461222586803179083557;
long double Y = 0.003322002718062184557764259218386616609;

// Pixel structure - position and color
struct Pixel {
  long double x;
  long double y;
  int color_r;
  int color_g;
  int color_b;
};

// Iteration of one pixel
int mandelbrot(long double x, long double y, int max_iteration) {
  long double tmp;
  int iteration = 0;

  // remeber starting value
  long double x0 = x;
  long double y0 = y;

  // While |x-iy|<4 and we haven't reach maximum iteration yet...
  while (x * x + y * y < 2 * 2 && iteration < max_iteration) {
    tmp = x * x - y * y + x0;
    // ...apply transformation:
    // x -> x^2 - y^2 + x0
    // y -> 2xy + y0
    y = 2 * x * y + y0;
    x = tmp;
    iteration++;
  }

  // if maximal iteration was crossed (so point converges) then the pixel is
  // black
  if (iteration >= max_iteration) return 0;

  // if not then return iteration after which convergance is broken (color will
  // depend on that values)
  else
    return iteration;
}

// xmin           xmax
//  |              |
//  V              V
//  ---------------- <- ymax
//  |      /---\   |
//  | -o--|     |  |
//  |      \---/   |
//  ---------------- <- ymin
//
// Plot one frame (whole Mandelbort set)
void mandelbrotset(string fileName, long double xmin, long double xmax,
                   long double ymin, long double ymax) {
  ofstream file;

  file.open(fileName.c_str());

  // Insert header
  file << "P3" << endl;
  file << "# Fraktal Mandelbota" << endl;
  file << W << " " << H << endl;
  file << alpha << endl;

  // Width and height of one pixel
  long double dW = (xmax - xmin) / (long double)W;
  long double dH = (ymax - ymin) / (long double)H;

  // For each pixel ...
  for (long double j = 0; j < H; j++)
    for (long double i = 0; i < W; i++) {
      Pixel pixel;
      int iteration;

      // calculate the coordinates of a pixel
      pixel.x = xmin + (dW * i);
      pixel.y = ymin + (dH * j);

      // calculate if it converges
      iteration = mandelbrot(pixel.x, pixel.y, max_iteration);

      // play with color (here you can try your ideas, personally I dont have
      // idea how to predict what Ill get)
      kappa = (double)iteration / (double)max_iteration;
      sigma = static_cast<int>(alpha * kappa);

      // if (kappa < 0.2) {
      // pixel.color_r = (sigma * 0.5);
      // pixel.color_g = (sigma * 0.75);
      // pixel.color_b = (sigma * 2);
      pixel.color_r = sigma * 1996 % 100;
      pixel.color_g = sigma * 966 % 180;
      pixel.color_b = sigma * 1410 % 255;
      // }

      // else if (kappa >= 0.2) {
      //   pixel.color_r = sigma % 32 * 8;
      //   pixel.color_g = sigma % 32 * 8;
      //   pixel.color_b = sigma % 128 * 2;
      // }

      // else
      // {
      // 	pixel.color_r = sigma % 8 * 32;
      // 	pixel.color_g = sigma % 8 * 32;
      // 	pixel.color_b = sigma % 16 * 16;
      // }

      // save pixel to file
      file << pixel.color_r << " " << pixel.color_g << " " << pixel.color_b
           << endl;
    }

  file.close();
}

// testting function for color palette
void palette(string fileName) {
  ofstream file;

  file.open(fileName.c_str());

  // Insert header
  file << "P3" << endl;
  file << "# Fraktal Mandelbota" << endl;
  file << 100 << " " << 100 << endl;
  file << alpha << endl;

  for (int sigma = 1; sigma <= 100; sigma += 1) {
    for (int j = 0; j < 100; ++j) {
      Pixel pixel;

      pixel.color_r = sigma * 1996 % 100;
      pixel.color_g = sigma * 966 % 180;
      pixel.color_b = sigma * 1410 % 255;

      file << pixel.color_r << " " << pixel.color_g << " " << pixel.color_b
           << " ";
    }
    file << endl;
  }
}

int main(int argc, char *argv[]) {
  ostringstream name;

  // Run a test -- print pallete of colors
  if (TEST) {
    palette("Test.ppm");
    return 0;
  }

  // first and last frame number (1 is maximally zoomed out, the higher number
  // gets the more zoomed it is; it breaks around 3000s)
  int begining = atoi(argv[1]);
  int end = atoi(argv[2]);

  // initial zoom
  ss = pow(s, begining);

  for (int i = begining; i < end; i++) {
    // zoom chcnges by zooming factor
    ss *= s;

    name.str("");

    if (i < 10) {
      name << "Pic/Fraktal_000" << i << ".ppm";
    } else if (i < 100) {
      name << "Pic/Fraktal_00" << i << ".ppm";
    } else if (i < 1000) {
      name << "Pic/Fraktal_0" << i << ".ppm";
    } else {
      name << "Pic/Fraktal_" << i << ".ppm";
    }

    cout << "Just made " << i << " fractal\n";

    // create frame
    mandelbrotset(name.str(), X - ss, X + ss, Y - ss, Y + ss);
  }

  return 0;
}
