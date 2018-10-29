#ifndef COLOR_TOOLS_H
#define COLOR_TOOLS_H

#include <QColor>
#include <math.h>
#include "color_rgba.h"
# define M_PI           3.14159265358979323846

namespace color {

class Tools
{
public:
    static void qcolor2rgba(const QColor& qColor, RGBA& rgba) {
        rgba.r = qColor.red();
        rgba.g = qColor.green();
        rgba.b = qColor.blue();
        rgba.a = qColor.alpha();
    }

    static void hsv_shift(const RGBA& rgba, float h_shift, float s_shift, float v_shift, RGBA& tr_rgba) {
        float VSU = v_shift*s_shift*cos(h_shift * M_PI/180);
        float VSW = v_shift*s_shift*sin(h_shift * M_PI/180);
        tr_rgba.r = static_cast<float>(.299*v_shift+.701*VSU+.168*VSW)*rgba.r
            + (.587*v_shift-.587*VSU+.330*VSW)*rgba.g
            + (.114*v_shift-.114*VSU-.497*VSW)*rgba.b;
        tr_rgba.g = (.299*v_shift-.299*VSU-.328*VSW)*rgba.r
            + (.587*v_shift+.413*VSU+.035*VSW)*rgba.g
            + (.114*v_shift-.114*VSU+.292*VSW)*rgba.b;
        tr_rgba.b = (.299*v_shift-.3*VSU+1.25*VSW)*rgba.r
            + (.587*v_shift-.588*VSU-1.05*VSW)*rgba.g
            + (.114*v_shift+.886*VSU-.203*VSW)*rgba.b;
        tr_rgba.r = tr_rgba.r < 0 ? 0 : tr_rgba.r;
        tr_rgba.r = tr_rgba.r > 255 ? 255 : tr_rgba.r;
        tr_rgba.g = tr_rgba.g < 0 ? 0 : tr_rgba.g;
        tr_rgba.g = tr_rgba.g > 255 ? 255 : tr_rgba.g;
        tr_rgba.b = tr_rgba.b < 0 ? 0 : tr_rgba.b;
        tr_rgba.b = tr_rgba.b > 255 ? 255 : tr_rgba.b;
    }
    static float qcolor2value(const QColor& qColor);
};
}

#endif // COLOR_TOOLS_H
