#ifndef COLOR_CONVERTER_H
#define COLOR_CONVERTER_H

#include "color_rgba.h"
#include "color_hsv.h"
#include "color_lab.h"

namespace color {

class Converter
{
private:
    float M[3][3] = {
        {0.5767309f, 0.1855540f, 0.1881852f},
        {0.2973769f, 0.6273491f, 0.0752741f},
        {0.0270343f, 0.0706872f, 0.9911085f}
    };
    static float lab_f(float x);
public:
    static void rgba2hsv(const RGBA& rgba, HSV& hsv);
    static void rgba2lab(const RGBA& rgba, LAB& lab);
};

}

#endif // COLOR_CONVERTER_H
