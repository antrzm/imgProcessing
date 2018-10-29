#include "imagefilter.h"

void ImageFilter::gauss(const QImage& input, QImage& output, int k, float sigma)
{
    /* Create a matrix for the convolution */
    if (k < 0)
    {
        k = 0;
    }
    int filter_side_size = 2*k + 1;
    vector<float> filter = vector<float>();
    vector<QPoint> offset = vector<QPoint>();
    for (int i = 0; i < filter_side_size; i++)
    {
        for (int j = 0; j < filter_side_size; j++)
        {
            int x = i - filter_side_size/2;
            int y = j - filter_side_size/2;
            filter.push_back(gauss_f(x, y, sigma));
            offset.push_back(QPoint(x, y));
        }
    }
    convolution(input, output, filter, offset);
}
void ImageFilter::sobel(const QImage& input, QImage& output)
{
}
void ImageFilter::habor(const QImage& input, QImage& output, int k, float sigma)
{
}





void ImageFilter::Blur(vector<float>& filter, vector<QPoint>& offsets, int k)
{
    if (k < 0)
    {
        k = 1;
    }
    if (k % 2 == 0)
    {
        k += 1;
    }
    filter.clear();
    offsets.clear();
    int offset = k / 2;
    for (int x = 0; x < k; x++)
    {
        for (int y = 0; y < k; y++)
        {
            filter.push_back(1.f/(k*k));
            offsets.push_back(QPoint(x - offset, y - offset));
        }
    }
}
void ImageFilter::Gauss(vector<float>& filter, vector<QPoint>& offsets, int k, float sigma)
{
    if (k < 0)
    {
        k = 1;
    }
    if (k % 2 == 0)
    {
        k += 1;
    }
    filter.clear();
    offsets.clear();
    int offset = k / 2;
    for (int x = 0; x < k; x++)
    {
        for (int y = 0; y < k; y++)
        {
            filter.push_back(gauss_f(x - offset, y - offset, sigma));
            offsets.push_back(QPoint(x - offset, y - offset));
        }
    }
}
void ImageFilter::convolution(const QImage& input, QImage& output, const vector<float>& filter, const vector<QPoint>& offsets)
{
    output = QImage(input.width(), input.height(), QImage::Format_RGB32);
    if (filter.size() == 0 || offsets.size() == 0 || filter.size() != offsets.size())
    {
        return;
    }
    for (int output_y = 0; output_y < output.height(); output_y++)
    {
        for (int output_x = 0; output_x < output.width(); output_x++)
        {
            int r = 0;
            int g = 0;
            int b = 0;
            int a = 0;
            for (int filter_i = 0; filter_i < filter.size(); filter_i++)
            {
                int input_y = (output_y + offsets[filter_i].y()) % input.height();
                if (input_y < 0) {
                    input_y += input.height();
                }
                int input_x = (output_x + offsets[filter_i].x()) % input.width();
                if (input_x < 0) {
                    input_x += input.width();
                }
                QColor input_color = input.pixelColor(input_x, input_y);
                r += (input_color.red() * filter[filter_i]);
                g += (input_color.green() * filter[filter_i]);
                b += (input_color.blue() * filter[filter_i]);
                a += (input_color.alpha() * filter[filter_i]);
            }
            if (r < 0)
            {
                r = 0;
            }
            if (g < 0)
            {
                g = 0;
            }
            if (b < 0)
            {
                b = 0;
            }
            if (a < 0)
            {
                a = 0;
            }
            if (r > 255)
            {
                r = 255;
            }
            if (g > 255)
            {
                g = 255;
            }
            if (b > 255)
            {
                b = 255;
            }
            if (a > 255)
            {
                a = 255;
            }
            output.setPixelColor(output_x, output_y, QColor(r, g, b, a));
        }
    }
}
