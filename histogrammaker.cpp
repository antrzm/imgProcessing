#include "histogrammaker.h"
#include <QImage>
#include <QColor>
#include "color_rgba.h"
#include "color_lab.h"
#include "color_tools.h"
#include "color_converter.h"

using namespace color;

std::vector<float> HistogramMaker::image_l_values(QImage image)
{
    std::vector<float> l_values = std::vector<float>();
    for (int x = 0; x < image.width(); x++)
    {
        for (int y = 0; y < image.height(); y++)
        {
            RGBA rgba;
            Tools::qcolor2rgba(QColor(image.pixelColor(x, y)), rgba);
            LAB lab;
            Converter::rgba2lab(rgba, lab);
            l_values.push_back(lab.l);
        }
    }
    return l_values;
}

QImage HistogramMaker::histogram(int height, int line_width, int border_width, int border_height, QColor background, QColor foreground, QColor border, std::vector<float> color_values, int lines, int min, int max)
{
    std::vector<float> values = std::vector<float>(lines);
    float diff = float(max - min) / lines;
    for (int i = 0; i < color_values.size(); i++)
    {
        int index = int((color_values[i] - min) / diff);
        if (index < 0)
        {
            index = 0;
        }
        if (index >= values.size())
        {
            index = values.size() - 1;
        }
        values[index] = values[index] + 1;
    }
    return histogram(height, line_width, border_width, border_height, background, foreground, border, values);
}
QImage HistogramMaker::histogram(int height, int line_width, int border_width, int border_height, QColor background, QColor foreground, QColor border, std::vector<float> color_values, int lines)
{
    float max = color_values[0];
    float min = color_values[0];
    for (int i = 0; i < color_values.size(); i++)
    {
        if (color_values[i] > max)
        {
            max = color_values[i];
        }
        if (color_values[i] < min)
        {
            min = color_values[i];
        }
    }
    return histogram(height, line_width, border_width, border_height, background, foreground, border, color_values, lines, min, max);
}
QImage HistogramMaker::histogram(int height, int line_width, int border_width, int border_height, QColor background, QColor foreground, QColor border, std::vector<float> values)
{
    float max_value = 1.0f;
    for(int i = 0; i < values.size(); i++)
    {
        if (values[i] > max_value)
        {
            max_value = values[i];
        }
    }
    for(int i = 0; i < values.size(); i++)
    {
        values[i] = values[i] / max_value;
    }
    int width = line_width * values.size() + border_width * (values.size() + 1);
    QImage image = QImage(width, height, QImage::Format_RGB32);
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            image.setPixelColor(x, y, background);
        }
    }
    int max_line_height = height - border_height;
    for (int i = 0; i < values.size(); i++)
    {
        int line_height = int(max_line_height * values[i]);
        // verical border
        for(int x = 0; x < border_width; x++)
        {
            for(int y = 0; y < line_height + border_height; y++)
            {
                image.setPixelColor(i * (border_width + line_width) + x, height - 1 - y, border);
                image.setPixelColor((i+1) * (border_width + line_width) + x, height - 1 - y, border);
            }
        }
        for(int x = 0; x < line_width; x++)
        {
            for(int y = 0; y < line_height; y++)
            {
                image.setPixelColor(i * (border_width + line_width) + border_width + x, height - 1 - y, foreground);
            }
            for(int y = line_height; y < line_height + border_height; y++)
            {
                image.setPixelColor(i * (border_width + line_width) + border_width + x, height - 1 - y, border);
            }
        }
    }
    return image;
}
