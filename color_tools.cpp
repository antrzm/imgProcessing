#include "color_tools.h"

namespace color {

float Tools::qcolor2value(const QColor& qColor)
{
    int red = qColor.red();
    int green = qColor.green();
    int blue = qColor.blue();
    int max = red;
    if (green > max)
    {
        max = green;
    }
    if (blue > max)
    {
        max = blue;
    }
    return max / 255.f;
}

}
