#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QImage>
#include <QString>
#include <QMutex>
#include "tabledialog.h"
#include "color_rgba.h"
#include "color_hsv.h"
#include "color_lab.h"
#include "filtersdialog.h"
#include "hsvtransformdialog.h"
#include "filter.h"
#include "imageeditor.h"

class QAction;
class QLabel;
class QMenu;
class QScrollArea;
class QScrollBar;
class QFileDialog;
class QDir;
class QMessageBox;

using namespace color;

namespace Ui {
    class MainWindow;
    class FiltersDialog;
    class HSVTransformDialog;
}

class MainWindow : public QMainWindow, ImageEditor {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void load_image(QString path);
    void update_image();
    void reset_image();

    void process_image(Filter* filter);
    void gauss(int window, float sigma);
    void sobel();
    void gabor(int window, float tetta);
    void canny();

    void hsv_transform(float h_shift, float s_mul, float v_mul);
    void try_hsv_transform(float h_shift, float s_mul, float v_mul);
    void finished();

    void pick_pixel(QPoint point);
    void pick_rectangle(QPoint point_0, QPoint point_1);
    void reset_pixel();
    void reset_rectangle();

    bool is_correct_x(const QImage& image, int x);
    bool is_correct_y(const QImage& image, int y);
    bool is_point_correct(const QImage& image, const QPoint& point);

    void try_pick_pixel();
    void try_pick_rectangle();

    void hsvTransformDialogIsClosed();
    void filtersDialogIsClosed();

    void open_rectangle_rgb_dialog();
    void open_rectangle_hsv_dialog();
    void open_rectangle_lab_dialog();

private slots:
    void on_actionOpen_triggered();
    void on_actionL_histogram_triggered();
    void on_actionHSV_Transform_triggered();
    void on_actionFilters_triggered();

    void on_rectangle_rgb_button_clicked();
    void on_rectangle_hsv_button_clicked();
    void on_rectangle_lab_button_clicked();
    void on_pixel_x_lineEdit_editingFinished();
    void on_pixel_y_lineEdit_editingFinished();
    void on_rectangle_x0_lineEdit_editingFinished();
    void on_rectangle_y0_lineEdit_editingFinished();
    void on_rectangle_x1_lineEdit_editingFinished();
    void on_rectangle_y1_lineEdit_editingFinished();

private:

    /* View & control */
    /* Ui */
    Ui::MainWindow *ui = nullptr;
    /* Dialogs */
    Ui::FiltersDialog* filtersDialog = nullptr;
    Ui::HSVTransformDialog* hsvTransformDialog = nullptr;
    /* Mutex */
    QMutex mutex;
    /* Image */
    QImage image;
    QImage preserved_image;
    /* Picked pixel */
    bool is_pixel_picked;
    QPoint pixel_point;
    /* Picked rectangle */
    bool is_rectangle_picked;
    QPoint rectangle_point_0;
    QPoint rectangle_point_1;

    /* View */
    /* 1. Menubar */
    /* 1.1. Image */
    void turn_on_menubar_image();
    void turn_off_menubar_image();
    /* 1.2. Edit */
    void turn_on_menubar_edit();
    void turn_off_menubar_edit();
    /* 2. Sidemenu */
    /* 2.1. Pixel */
    void turn_on_sidemenu_pixel();
    void turn_off_sidemenu_pixel();
    void set_sidemenu_pixel(QPoint point, RGBA rgba, HSV hsv, LAB lab);
    void reset_sidemenu_pixel();
    /* 2.2. Rectangle */
    void turn_on_sidemenu_rectangle();
    void turn_on_sidemenu_rectangle_rgb_hsv_lab_buttons();
    void turn_off_sidemenu_rectangle();
    void set_sidemenu_rectangle(QPoint point_0, QPoint point_1);
    void reset_sidemenu_rectangle();
    /* ---------- */
};

#endif
