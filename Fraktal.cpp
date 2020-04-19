//**************************
// Mandelbrot Fractal
//**************************
// Change to 1 if you want to print only palette
#define TEST 0
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// Resolution of image
const int WIDTH = 1920;
const int HEIGHT = 1080;

// Maximal color
const int max_color_value = 255;
// Maximal iteration
const int max_iteration = 10000;
// zoom factor
const long double zoom_factor = 0.97;

// Position of point we are zooming in
// long double X = -0.789374599271466936740382412558;
// long double Y = 0.163089252677526719026415054868;
long double X = -1.477110786384222313461222586803179083557;
long double Y = 0.003322002718062184557764259218386616609;

// abs definition
float abs(float a) {
  if (a >= 0)
    return a;
  else
    return -a;
}

// Pixel structure - position and color
struct Pixel {
  // position
  long double x;
  long double y;
  // hue, saturation and value
  float color_h;
  float color_s;
  float color_v;
  // converetr to RGB
  int color_r() {
    float r = 0;

    if (color_h >= 0 and color_h < 60) {
      r += color_v * color_s;
    } else if (color_h >= 60 and color_h < 120) {
      r += color_v * color_s * (1 - abs(fmod(color_h / 60.0, 2) - 1));
    } else if (color_h >= 120 and color_h < 180) {
      r += 0;
    } else if (color_h >= 180 and color_h < 240) {
      r += 0;
    } else if (color_h >= 240 and color_h < 300) {
      r += color_v * color_s * (1 - abs(fmod(color_h / 60.0, 2) - 1));
    } else if (color_h >= 300 and color_h <= 360) {
      r += color_v * color_s;
    }

    r += color_v - color_v * color_s;

    return r * 255;
  }
  int color_g() {
    float g = 0;

    if (color_h >= 0 and color_h < 60) {
      g += color_v * color_s * (1 - abs(fmod(color_h / 60.0, 2) - 1));
    } else if (color_h >= 60 and color_h < 120) {
      g += color_v * color_s;
    } else if (color_h >= 120 and color_h < 180) {
      g += color_v * color_s;
    } else if (color_h >= 180 and color_h < 240) {
      g += color_v * color_s * (1 - abs(fmod(color_h / 60.0, 2) - 1));
    } else if (color_h >= 240 and color_h < 300) {
      g += 0;
    } else if (color_h >= 300 and color_h <= 360) {
      g += 0;
    }

    g += color_v - color_v * color_s;

    return g * 255;
  };
  int color_b() {
    float b = 0;

    if (color_h >= 0 and color_h < 60) {
      b += 0;
    } else if (color_h >= 60 and color_h < 120) {
      b += 0;
    } else if (color_h >= 120 and color_h < 180) {
      b += color_v * color_s * (1 - abs(fmod(color_h / 60.0, 2) - 1));
    } else if (color_h >= 180 and color_h < 240) {
      b += color_v * color_s;
    } else if (color_h >= 240 and color_h < 300) {
      b += color_v * color_s;
    } else if (color_h >= 300 and color_h <= 360) {
      b += color_v * color_s * (1 - abs(fmod(color_h / 60.0, 2) - 1));
    }

    b += color_v - color_v * color_s;

    return b * 255;
  };
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

  // Insert header (nedded for .ppm files)
  file << "P3" << endl;
  file << "# Fraktal Mandelbota" << endl;
  file << WIDTH << " " << HEIGHT << endl;
  file << max_color_value << endl;

  // Width and height of one pixel
  long double dWIDTH = (xmax - xmin) / (long double)WIDTH;
  long double dHEIGHT = (ymax - ymin) / (long double)HEIGHT;

  // For each pixel ...
  for (long double j = 0; j < HEIGHT; j++)
    for (long double i = 0; i < WIDTH; i++) {
      Pixel pixel;
      int iteration;

      // calculate the coordinates of a pixel
      pixel.x = xmin + (dWIDTH * i);
      pixel.y = ymin + (dHEIGHT * j);

      // calculate if it converges
      iteration = mandelbrot(pixel.x, pixel.y, max_iteration);

      // play with color (here you can try your ideas, personally I dont have
      // idea how to predict what Ill get)
      double kappa = (double)iteration / (double)max_iteration;

      // coloring in RGB
      // int sigma = static_cast<int>(max_color_value * kappa);
      //
      // pixel.color_r = sigma * 1612 % 180;
      // pixel.color_g = sigma * 1905 % 220;
      // pixel.color_b = sigma * 1917 % 255;

      // coloring in HSV
      pixel.color_h = 180;
      pixel.color_s = 1;
      pixel.color_v = fmod(kappa * 10, 10);

      // save pixel to file -- always n RGB
      file << pixel.color_r() << " " << pixel.color_g() << " "
           << pixel.color_b() << endl;
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
  file << max_color_value << endl;

  for (float sigma = 1; sigma <= 360; sigma += 1) {
    for (int j = 0; j < 100; ++j) {
      Pixel pixel;

      // pixel.color_r = sigma * 1996 % 100;
      // pixel.color_g = sigma * 966 % 180;
      // pixel.color_b = sigma * 1410 % 255;

      pixel.color_h = sigma;
      pixel.color_s = 1;
      pixel.color_v = 1;

      file << pixel.color_r() << " " << pixel.color_g() << " "
           << pixel.color_b() << " ";
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
  long double current_zoom = pow(zoom_factor, begining);

  for (int i = begining; i < end; i++) {
    // zoom chcnges by zooming factor
    current_zoom *= zoom_factor;

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
    mandelbrotset(name.str(), X - current_zoom, X + current_zoom,
                  Y - current_zoom, Y + current_zoom);
  }

  return 0;
}
