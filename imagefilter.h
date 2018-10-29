#ifndef IMAGEFILTER_H
#define IMAGEFILTER_H

#include <QImage>
#include "math.h"

#define M_PI 3.14159265358979323846

using namespace std;

class ImageFilter
{
public:
    static void gauss(const QImage& input, QImage& output, int k, float sigma);
    static void sobel(const QImage& input, QImage& output);
    static void habor(const QImage& input, QImage& output, int k, float sigma);

    static float gauss_f(float x, float y, float sigma)
    {
        return 1./(2.*M_PI*sigma*sigma)*exp(-(x*x + y*y)/(2*sigma*sigma));
    }
    static void Blur(vector<float>& filter, vector<QPoint>& offsets, int k);
    static void Gauss(vector<float>& filter, vector<QPoint>& offsets, int k, float sigma);
    static void convolution(const QImage& input, QImage& output, const vector<float>& filter, const vector<QPoint>& offsets);
};

#endif // IMAGEFILTER_H
