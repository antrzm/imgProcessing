#include "imagelabel.h"
#include <iostream>
#include <QPoint>

ImageLabel::ImageLabel(QWidget* parent): QLabel(parent)
{
    setMouseTracking(true);
}

void ImageLabel::set_MainWindow(MainWindow* mainWindow)
{
    this->mainWindow = mainWindow;
}

void ImageLabel::mousePressEvent(QMouseEvent* ev)
{
    if (ev->button() == Qt::LeftButton)
    {
        is_left_button_pressed = true;
    }
    if (ev->button() == Qt::RightButton)
    {
        is_right_button_pressed = true;
        rectangle_point_0 = ev->localPos().toPoint();
        if (mainWindow == nullptr)
        {
            return;
        }
        mainWindow->pick_rectangle(rectangle_point_0, rectangle_point_0);
    }
}

void ImageLabel::mouseMoveEvent(QMouseEvent* ev)
{
    if (mainWindow == nullptr)
    {
        return;
    }
    if (is_left_button_pressed)
    {
        mainWindow->pick_pixel(ev->localPos().toPoint());
    }
    if (is_right_button_pressed)
    {
        mainWindow->pick_rectangle(rectangle_point_0, ev->localPos().toPoint());
    }
}

void ImageLabel::mouseReleaseEvent(QMouseEvent* ev)
{
    is_left_button_pressed = false;
    is_right_button_pressed = false;
}
