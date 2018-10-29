#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include "mainwindow.h"

class ImageLabel: public QLabel
{
private:
    MainWindow* mainWindow = nullptr;
    bool is_left_button_pressed = false;
    bool is_right_button_pressed = false;
    QPoint rectangle_point_0;
public:
    ImageLabel(QWidget* parent);
    void set_MainWindow(MainWindow* mainWindow);

    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
};

#endif // IMAGELABEL_H
