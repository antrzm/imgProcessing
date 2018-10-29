#ifndef HISTOGRAMMAKER_H
#define HISTOGRAMMAKER_H

#include <QImage>
#include <QColor>
#include <vector>

class HistogramMaker
{
public:
    static std::vector<float> image_l_values(QImage image);
//    static std::vector<float> image_a_values(QImage image);
//    static std::vector<float> image_b_values(QImage image);
    static QImage histogram(int height, int line_width, int border_width, int border_height, QColor background, QColor foreground, QColor border, std::vector<float> color_values, int lines);
    static QImage histogram(int height, int line_width, int border_width, int border_height, QColor background, QColor foreground, QColor border, std::vector<float> color_values, int lines, int min, int max);
    static QImage histogram(int height, int line_width, int border_width, int border_height, QColor background, QColor foreground, QColor border, std::vector<float> values);
};


#endif // HISTOGRAMMAKER_H
