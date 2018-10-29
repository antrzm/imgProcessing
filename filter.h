#ifndef FILTER_H
#define FILTER_H

#include <QImage>
#include <QPoint>
#include <vector>

using namespace std;

class Filter
{
public:
    virtual void go(const QImage& input, QImage& output) = 0;
};

class Gauss: public Filter
{
private:
    vector<float> gauss_k;
    vector<QPoint> offset;
    static float gauss_function(float x, float y, float sigma);
public:
    Gauss(int window, float sigma);
    void go(const QImage& input, QImage& output);
};


class Sobel: public Filter
{
private:
    vector<QPoint> offset;
    vector<float> grad_x_k;
    vector<float> grad_y_k;
public:
    Sobel();
    void go(const QImage& input, QImage& output);
};

class Gabor: public Filter
{
    vector<float> gabor_k;
    vector<QPoint> offset;
    static float gabor_function(int x, int y, float tetta);
public:
    Gabor(int window, int tetta);
    void go(const QImage& input, QImage& output);
};

class HsvTransform: public Filter
{
private:
    float h_shift;
    float s_mul;
    float v_mul;
public:
    HsvTransform(float h_shift, float s_mul, float v_mul);
    void go(const QImage& input, QImage& output);
};

class Canny: public Filter{
private:
    vector<QPoint> offset;
    vector<float> grad_x_k;
    vector<float> grad_y_k;
public:
    Canny();
    void go(const QImage& input, QImage& output);
};

void trace(int width, int height, QVector<int> &image, int x, int y);
inline QVector<int> threshold(int thLow, int thHi, const QVector<int> &image);
void sobel_for_canny(const QImage &image, QVector<int> &gradient, QVector<int> &direction);

#endif // FILTER_H
