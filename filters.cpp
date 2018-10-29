#include "filter.h"
#include "math.h"
#include "color_tools.h"

float Gauss::gauss_function(float x, float y, float sigma)
{
    return 1./(2.*M_PI*sigma*sigma)*exp(-(x*x + y*y)/(2*sigma*sigma));
}
Gauss::Gauss(int window, float sigma)
{
    if (window < 1)
    {
        window = 1;
    }
    if (window%2 == 0)
    {
        window += 1;
    }
    for (int i = 0; i < window; i++)
    {
        int offset_x = i - window/2;
        for (int j = 0; j < window; j++)
        {
            int offset_y = j - window/2;
            offset.push_back(QPoint(offset_x, offset_y));
            gauss_k.push_back(gauss_function(offset_x, offset_y, sigma));
        }
    }
    float sum = 0;
    for(int i = 0; i < gauss_k.size(); i++)
    {
        sum += gauss_k[i];
    }
    for(int i = 0; i < gauss_k.size(); i++)
    {
        gauss_k[i] = gauss_k[i] / sum;
    }
}
void Gauss::go(const QImage& input, QImage& output) {
    output = QImage(input.width(), input.height(), QImage::Format_RGB32);
    for (int output_x = 0; output_x < output.width(); output_x++)
    {
        for (int output_y = 0; output_y < output.height(); output_y++)
        {
            float output_r = 0;
            float output_g = 0;
            float output_b = 0;
            float output_a = 0;
            for (int i = 0; i < offset.size(); i++)
            {
                int input_x = output_x + offset[i].x();
                if (input_x < 0)
                {
                    input_x = 0;
                }
                if (input_x >= input.width())
                {
                    input_x = input.width() - 1;
                }
                int input_y = output_y + offset[i].y();
                if (input_y < 0)
                {
                    input_y = 0;
                }
                if (input_y >= input.height())
                {
                    input_y = input.height() - 1;
                }
                QColor input_color = QColor(input.pixelColor(input_x, input_y));
                output_r += (input_color.red() * gauss_k[i]);
                output_g += (input_color.green() * gauss_k[i]);
                output_b += (input_color.blue() * gauss_k[i]);
                output_a += (input_color.alpha() * gauss_k[i]);
            }
            if (output_r < 0) { output_r = 0; }
            if (output_r > 255) { output_r = 255; }
            if (output_g < 0) { output_g = 0; }
            if (output_g > 255) { output_g = 255; }
            if (output_b < 0) { output_b = 0; }
            if (output_b > 255) { output_b = 255; }
            if (output_a < 0) { output_a = 0; }
            if (output_a > 255) { output_a = 255; }
            output.setPixelColor(output_x, output_y, QColor(output_r, output_g, output_b, output_a));
        }
    }
}

Sobel::Sobel()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            offset.push_back(QPoint(i - 1, j - 1));
        }
    }
    grad_x_k.push_back(-1);
    grad_x_k.push_back(-2);
    grad_x_k.push_back(-1);
    grad_x_k.push_back(0);
    grad_x_k.push_back(0);
    grad_x_k.push_back(0);
    grad_x_k.push_back(1);
    grad_x_k.push_back(2);
    grad_x_k.push_back(1);
    grad_y_k.push_back(-1);
    grad_y_k.push_back(0);
    grad_y_k.push_back(1);
    grad_y_k.push_back(-2);
    grad_y_k.push_back(0);
    grad_y_k.push_back(2);
    grad_y_k.push_back(-1);
    grad_y_k.push_back(0);
    grad_y_k.push_back(1);
}
void Sobel::go(const QImage& input, QImage& output)
{
    output = QImage(input.width(), input.height(), QImage::Format_RGB32);
    for (int output_x = 0; output_x < output.width(); output_x++)
    {
        for (int output_y = 0; output_y < output.height(); output_y++)
        {
            float grad_x_value = 0;
            float grad_y_value = 0;
            for (int i = 0; i < offset.size(); i++)
            {
                int input_x = output_x + offset[i].x();
                if (input_x < 0)
                {
                    input_x = 0;
                }
                if (input_x >= input.width())
                {
                    input_x = input.width() - 1;
                }
                int input_y = output_y + offset[i].y();
                if (input_y < 0)
                {
                    input_y = 0;
                }
                if (input_y >= input.height())
                {
                    input_y = input.height() - 1;
                }
                float input_value = color::Tools::qcolor2value(QColor(input.pixelColor(input_x, input_y)));
                grad_x_value += (input_value * grad_x_k[i]);
                grad_y_value += (input_value * grad_y_k[i]);
            }
            int output_value = int(sqrt(pow(grad_x_value, 2.) + pow(grad_y_value, 2.))*255.f + 0.5);
            if (output_value < 0)
            {
                output_value = 0;
            }
            if (output_value > 255)
            {
                output_value = 255;
            }
            output.setPixelColor(output_x, output_y, QColor(output_value, output_value, output_value));
        }
    }
}


float Gabor::gabor_function(int x, int y, float tetta)
{
    tetta *= (M_PI/180);
    float x_1 = x*cos(tetta) + y*sin(tetta);
    float y_1 = -x*sin(tetta) + y*cos(tetta);
    float gamma = 1.;
    float lambda = 2.;
    float sigma = lambda*0.56;
    float fi = 0*M_PI/180;
    return exp(-0.5*(x_1*x_1 + gamma*gamma*y_1*y_1)/sigma) *cos(2*M_PI*x_1/lambda + fi);
}
Gabor::Gabor(int window, int tetta)
{
    if (window < 1)
    {
        window = 1;
    }
    if (window%2 == 0)
    {
        window += 1;
    }
    for (int i = 0; i < window; i++)
    {
        int offset_x = i - window/2;
        for (int j = 0; j < window; j++)
        {
            int offset_y = j - window/2;
            offset.push_back(QPoint(offset_x, offset_y));
            gabor_k.push_back(gabor_function(offset_x, offset_y, tetta));
        }
    }
    for(int i = 0; i < gabor_k.size(); i++)
    {
        //gabor_k[i] = gabor_k[i]/(window*window);
    }
}
void Gabor::go(const QImage& input, QImage& output)
{
    output = QImage(input.width(), input.height(), QImage::Format_RGB32);
    for (int output_x = 0; output_x < output.width(); output_x++)
    {
        for (int output_y = 0; output_y < output.height(); output_y++)
        {
            float output_value = 0;
            for (int i = 0; i < offset.size(); i++)
            {
                int input_x = output_x + offset[i].x();
                if (input_x < 0)
                {
                    input_x = 0;
                }
                if (input_x >= input.width())
                {
                    input_x = input.width() - 1;
                }
                int input_y = output_y + offset[i].y();
                if (input_y < 0)
                {
                    input_y = 0;
                }
                if (input_y >= input.height())
                {
                    input_y = input.height() - 1;
                }
                float input_value = color::Tools::qcolor2value(QColor(input.pixelColor(input_x, input_y)));
                output_value += (input_value * gabor_k[i]);
            }
            output_value *= 255;
            if (output_value < 0)
            {
                output_value = 0;
            }
            if (output_value > 255)
            {
                output_value = 255;
            }
            output.setPixelColor(output_x, output_y, QColor(int(output_value), int(output_value), int(output_value)));
        }
    }
}

Canny::Canny(){}

inline QVector<int> thinning(int width, int height, const QVector<int> &gradient, const QVector<int> &direction)
{
    QVector<int> thinned(gradient.size());
    for (int y = 0; y < height; y++) {
        int yOffset = y * width;
        const int *gradientLine = gradient.constData() + yOffset;
        const int *gradientLine_m1 = y < 1? gradientLine: gradientLine - width;
        const int *gradientLine_p1 = y >= height - 1? gradientLine: gradientLine + width;
        const int *directionLine = direction.constData() + yOffset;
        int *thinnedLine = thinned.data() + yOffset;

        for (int x = 0; x < width; x++) {
            int x_m1 = x < 1? 0: x - 1;
            int x_p1 = x >= width - 1? x: x + 1;

            int direction = directionLine[x];
            int pixel = 0;

            if (direction == 0) {
                /* x x x
                 * - - -
                 * x x x
                 */
                if (gradientLine[x] < gradientLine[x_m1] || gradientLine[x] < gradientLine[x_p1])
                    pixel = 0;
                else
                    pixel = gradientLine[x];
            } else if (direction == 1) {
                if (gradientLine[x] < gradientLine_m1[x_p1] || gradientLine[x] < gradientLine_p1[x_m1])
                    pixel = 0;
                else
                    pixel = gradientLine[x];
            } else if (direction == 2) {
                if (gradientLine[x] < gradientLine_m1[x_m1] || gradientLine[x] < gradientLine_p1[x_p1])
                    pixel = 0;
                else
                    pixel = gradientLine[x];
            } else {
                if (gradientLine[x] < gradientLine_m1[x]
                    || gradientLine[x] < gradientLine_p1[x])
                    pixel = 0;
                else
                    pixel = gradientLine[x];
            }
            thinnedLine[x] = pixel;
        }
    }

    return thinned;
}

inline QVector<int> threshold(int thLow, int thHi, const QVector<int> &image)
{
    QVector<int> thresholded(image.size());

    for (int i = 0; i < image.size(); i++)
        thresholded[i] = image[i] <= thLow? 0:
                         image[i] >= thHi? 255:
                                           127;

    return thresholded;
}

void trace(int width, int height, QVector<int> &image, int x, int y)
{
    int yOffset = y * width;
    int *cannyLine = image.data() + yOffset;

    if (cannyLine[x] != 255)
        return;

    for (int j = -1; j < 2; j++) {
        int nextY = y + j;

        if (nextY < 0 || nextY >= height)
            continue;

        int *cannyLineNext = cannyLine + j * width;

        for (int i = -1; i < 2; i++) {
            int nextX = x + i;

            if (i == j || nextX < 0 || nextX >= width)
                continue;

            if (cannyLineNext[nextX] == 127) {
                // Mark pixel as white.
                cannyLineNext[nextX] = 255;

                // Trace neighbors.
                trace(width, height, image, nextX, nextY);
            }
        }
    }
}

QVector<int> hysteresis(int width, int height,
                        const QVector<int> &image)
{
    QVector<int> canny(image);

    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            trace(width, height, canny, x, y);

    // Remaining gray pixels becomes black.
    for (int i = 0; i < canny.size(); i++)
        if (canny[i] == 127)
            canny[i] = 0;

    return canny;
}

void sobel_for_canny(const QImage &image, QVector<int> &gradient, QVector<int> &direction){
    int size = image.width() * image.height();
        gradient.resize(size);
        direction.resize(size);

        for (int y = 0; y < image.height(); y++) {
            size_t yOffset = y * image.width();
            const quint8 *grayLine = image.constBits() + yOffset;

            const quint8 *grayLine_m1 = y < 1? grayLine: grayLine - image.width();
            const quint8 *grayLine_p1 = y >= image.height() - 1? grayLine: grayLine + image.width();

            int *gradientLine = gradient.data() + yOffset;
            int *directionLine = direction.data() + yOffset;

            for (int x = 0; x < image.width(); x++) {
                int x_m1 = x < 1? x: x - 1;
                int x_p1 = x >= image.width() - 1? x: x + 1;

                int gradX = grayLine_m1[x_p1]
                          + 2 * grayLine[x_p1]
                          + grayLine_p1[x_p1]
                          - grayLine_m1[x_m1]
                          - 2 * grayLine[x_m1]
                          - grayLine_p1[x_m1];

                int gradY = grayLine_m1[x_m1]
                          + 2 * grayLine_m1[x]
                          + grayLine_m1[x_p1]
                          - grayLine_p1[x_m1]
                          - 2 * grayLine_p1[x]
                          - grayLine_p1[x_p1];

                gradientLine[x] = qAbs(gradX) + qAbs(gradY);

                if (gradX == 0 && gradY == 0)
                    directionLine[x] = 0;
                else if (gradX == 0)
                    directionLine[x] = 3;
                else {
                    qreal a = 180. * atan(qreal(gradY) / gradX) / M_PI;

                    if (a >= -22.5 && a < 22.5)
                        directionLine[x] = 0;
                    else if (a >= 22.5 && a < 67.5)
                        directionLine[x] = 1;
                    else if (a >= -67.5 && a < -22.5)
                        directionLine[x] = 2;
                    else
                        directionLine[x] = 3;
                }
            }
        }
}

void Canny::go(const QImage& input, QImage& output)
{
    QImage inImage = input.convertToFormat(QImage::Format_Grayscale8);
    output = QImage(input.width(), input.height(), inImage.format());

    QVector<int> gradient;
    QVector<int> direction;
    sobel_for_canny(inImage, gradient, direction);
    QVector<int> thinned = thinning(inImage.width(), inImage.height(), gradient, direction);
    QVector<int> thresholded = threshold(75, 150, thinned);
    QVector<int> canny = hysteresis(inImage.width(), inImage.height(), thresholded);

    const int *iImg = canny.constData();
    quint8 *oImg = output.bits();

    int size = inImage.width() * inImage.height();

    for (int i = 0; i < size; i++)
        oImg[i] = static_cast<quint8>(qBound(0, iImg[i], 255));
}

HsvTransform::HsvTransform(float h_shift, float s_mul, float v_mul)
{
    this->h_shift = h_shift;
    this->s_mul = s_mul;
    this->v_mul = v_mul;
}
void HsvTransform::go(const QImage& input, QImage& output)
{
    output = QImage(input.width(), input.height(), QImage::Format_RGB32);
    for (int output_x = 0; output_x < output.width(); output_x++)
    {
        for (int output_y = 0; output_y < output.height(); output_y++)
        {
            color::RGBA rgba;
            color::Tools::qcolor2rgba(QColor(input.pixelColor(output_x, output_y)), rgba);
            color::RGBA tr_rgba;
            float VSU = v_mul*s_mul*cos(h_shift*M_PI/180);
            float VSW = v_mul*s_mul*sin(h_shift*M_PI/180);
            tr_rgba.r = (.299*v_mul+.701*VSU+.168*VSW)*rgba.r
                + (.587*v_mul-.587*VSU+.330*VSW)*rgba.g
                + (.114*v_mul-.114*VSU-.497*VSW)*rgba.b;
            tr_rgba.g = (.299*v_mul-.299*VSU-.328*VSW)*rgba.r
                + (.587*v_mul+.413*VSU+.035*VSW)*rgba.g
                + (.114*v_mul-.114*VSU+.292*VSW)*rgba.b;
            tr_rgba.b = (.299*v_mul-.3*VSU+1.25*VSW)*rgba.r
                + (.587*v_mul-.588*VSU-1.05*VSW)*rgba.g
                + (.114*v_mul+.886*VSU-.203*VSW)*rgba.b;
            tr_rgba.r = tr_rgba.r < 0 ? 0 : tr_rgba.r;
            tr_rgba.r = tr_rgba.r > 255 ? 255 : tr_rgba.r;
            tr_rgba.g = tr_rgba.g < 0 ? 0 : tr_rgba.g;
            tr_rgba.g = tr_rgba.g > 255 ? 255 : tr_rgba.g;
            tr_rgba.b = tr_rgba.b < 0 ? 0 : tr_rgba.b;
            tr_rgba.b = tr_rgba.b > 255 ? 255 : tr_rgba.b;
            tr_rgba.a = rgba.a;
            output.setPixelColor(output_x, output_y, QColor(tr_rgba.r, tr_rgba.g, tr_rgba.b, tr_rgba.a));
        }
    }
}
