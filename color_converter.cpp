#include "color_converter.h"
#include "math.h"

namespace color {

float Converter::lab_f(float x)
{
    return (x > pow(6.0/29.0, 3.0)) ? float(pow(x, 1.0/3.0)) : float(7.787 * x + 16.0/116.0);
}

void Converter::rgba2hsv(const RGBA& rgba, HSV& hsv)
{
    float r = float(rgba.r) / 255;
    float g = float(rgba.g) / 255;
    float b = float(rgba.b) / 255;
    float min = r;
    if (g < min)
    {
        min = g;
    }
    if (b < min)
    {
        min = b;
    }
    float max = r;
    if (g > max)
    {
        max = g;
    }
    if (b > max)
    {
        max = b;
    }

    // h
    if (min == max)
    {
        hsv.h = 0;
    }
    else if (max == r && g >= b)
    {
        hsv.h = 60 * (g - b) / (max - min);
    }
    else if (max == r && g < b)
    {
        hsv.h = 60 * (g - b) / (max - min) + 360;
    }
    else if (max == g)
    {
        hsv.h = 60 * (b - r) / (max - min) + 120;
    }
    else if (max == b)
    {
        hsv.h = 60 * (r - g) / (max - min);
    }
    else
    {
        throw 1;
    }

    // s
    if (max == 0)
    {
        hsv.s = 0;
    }
    else
    {
        hsv.s = 1 - (min / max);
    }

    // v
    hsv.v = max;
}

void Converter::rgba2lab(const RGBA& rgba, LAB& lab) {
    float r = rgba.r / 255.0f;
    float g = rgba.g / 255.0f;
    float b = rgba.b / 255.0f;

    r = (r > 0.04045) ? pow(((r + 0.055) / 1.055), 2.4) : r/12.92;
    g = (g > 0.04045) ? pow(((g + 0.055) / 1.055), 2.4) : g/12.92;
    b = (b > 0.04045) ? pow(((b + 0.055) / 1.055), 2.4) : b/12.92;

    r *= 100;
    g *= 100;
    b *= 100;

    float x = r * 0.4124 + g * 0.3576 + b * 0.1805;
    float y = r * 0.2126 + g * 0.7152 + b * 0.0722;
    float z = r * 0.0193 + g * 0.1192 + b * 0.9505;

    float xn = 95.047f;
    float yn = 100.000f;
    float zn = 108.883f;

    lab.l = 116 * lab_f(y/yn) - 16;
    lab.a = 500 * (lab_f(x/xn) - lab_f(y/yn));
    lab.b = 200 * (lab_f(y/yn) - lab_f(z/zn));
}

}
