#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "color_rgba.h"
#include "color_hsv.h"
#include "color_lab.h"
#include "color_tools.h"
#include "color_converter.h"
#include "tabledialog.h"
#include "imagedialog.h"
#include "histogrammaker.h"
#include <QColor>
#include "imagefilter.h"
#include "hsvtransformdialog.h"
#include "filtersdialog.h"

/* Constructor and destructor */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->imageLabel->set_MainWindow(this);
    this->setStyleSheet("color: darkCyan;" "background-color: black;");
    turn_off_menubar_image();
    turn_off_menubar_edit();
    turn_off_sidemenu_pixel();
    reset_pixel();
    load_image("C:\\Users\\Anton\\Documents\\QT projects\\imgTask1\\images.jpg");
}
MainWindow::~MainWindow()
{
    delete ui;
}


/* Image */
void MainWindow::load_image(QString path)
{
    mutex.lock();
    QImage new_image = QImage(path);
    if (new_image.isNull())
    {
        mutex.unlock();
        return;
    }
    image = new_image;
    preserved_image = QImage();
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    reset_pixel();
    // menubar
    turn_on_menubar_image();
    turn_on_menubar_edit();
    // sidemenu
    turn_on_sidemenu_pixel();
    mutex.unlock();
}
void MainWindow::update_image()
{
    mutex.lock();
    preserved_image = QImage();
    reset_pixel();
    mutex.unlock();
}
void MainWindow::reset_image()
{
    mutex.lock();
    if (preserved_image.isNull())
    {
        mutex.unlock();
        return;
    }
    image = preserved_image;
    preserved_image = QImage();
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    mutex.unlock();
}
/* Image processing */
void MainWindow::process_image(Filter* filter)
{
    mutex.lock();
    if (image.isNull())
    {
        mutex.unlock();
        return;
    }
    if (preserved_image.isNull())
    {
        preserved_image = image;
    }
    QImage new_image;
    filter->go(image, new_image);
    image = new_image;
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    mutex.unlock();
}
void MainWindow::gauss(int window, float sigma)
{
    Gauss filter = Gauss(window, sigma);
    process_image(&filter);
}
void MainWindow::sobel()
{
    Sobel filter = Sobel();
    process_image(&filter);
}
void MainWindow::gabor(int window, float tetta)
{
    Gabor filter = Gabor(window, tetta);
    process_image(&filter);
}

void MainWindow::canny(){
    Canny filter = Canny();
    process_image(&filter);
}

void MainWindow::hsv_transform(float h_shift, float s_mul, float v_mul)
{
    HsvTransform filter = HsvTransform(h_shift, s_mul, v_mul);
    process_image(&filter);
}
void MainWindow::try_hsv_transform(float h_shift, float s_mul, float v_mul)
{
    mutex.lock();
    if (image.isNull())
    {
        mutex.unlock();
        return;
    }
    if (preserved_image.isNull())
    {
        preserved_image = image;
    }
    QImage new_image;
    HsvTransform(h_shift, s_mul, v_mul).go(preserved_image, new_image);
    image = new_image;
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
    mutex.unlock();
}
void MainWindow::finished()
{
    ui->menuEdit->setEnabled(true);
}

/* Pixel */
void MainWindow::pick_pixel(QPoint point)
{
    if (image.isNull())
    {
        return;
    }
    if (!is_point_correct(image, point))
    {
        return;
    }
    is_pixel_picked = true;
    pixel_point = point;
    turn_on_sidemenu_pixel();
    RGBA rgba;
    Tools::qcolor2rgba(QColor(image.pixelColor(pixel_point)), rgba);
    HSV hsv;
    Converter::rgba2hsv(rgba, hsv);
    LAB lab;
    Converter::rgba2lab(rgba, lab);
    set_sidemenu_pixel(pixel_point, rgba, hsv, lab);
}
void MainWindow::reset_pixel()
{
    is_pixel_picked = false;
    pixel_point = QPoint();
    reset_sidemenu_pixel();
}


bool MainWindow::is_correct_x(const QImage& image, int x)
{
    return x >= 0 && x < image.width();
}
bool MainWindow::is_correct_y(const QImage& image, int y)
{
    return y >= 0 && y < image.height();
}
bool MainWindow::is_point_correct(const QImage& image, const QPoint& point)
{
    return is_correct_x(image, point.x()) && is_correct_y(image, point.y());
}


/* View */
/* 1. Menubar */
/* 1.1. Image */
void MainWindow::turn_on_menubar_image()
{
    ui->menuImage->setEnabled(true);
}
void MainWindow::turn_off_menubar_image()
{
    ui->menuImage->setEnabled(false);
}
/* 1.2. Edit */
void MainWindow::turn_on_menubar_edit()
{
    ui->menuEdit->setEnabled(true);
}
void MainWindow::turn_off_menubar_edit()
{
    ui->menuEdit->setEnabled(false);
}
/* 2. Sidemenu */
/* 2.1. Pixel */
void MainWindow::turn_on_sidemenu_pixel()
{
    ui->pixel_groupBox->setEnabled(true);
}
void MainWindow::turn_off_sidemenu_pixel()
{
    ui->pixel_groupBox->setEnabled(false);
}
void MainWindow::set_sidemenu_pixel(QPoint point, RGBA rgba, HSV hsv, LAB lab)
{
    ui->pixel_x_lineEdit->setText(QString::number(point.x()));
    ui->pixel_y_lineEdit->setText(QString::number(point.y()));
    ui->pixel_rgb_r_label->setText(QString::number(rgba.r));
    ui->pixel_rgb_g_label->setText(QString::number(rgba.g));
    ui->pixel_rgb_b_label->setText(QString::number(rgba.b));
    ui->pixel_hsv_h_label->setText(QString().sprintf("%.2f", hsv.h));
    ui->pixel_hsv_s_label->setText(QString().sprintf("%.2f", hsv.s));
    ui->pixel_hsv_v_label->setText(QString().sprintf("%.2f", hsv.v));
    ui->pixel_lab_l_label->setText(QString().sprintf("%.2f", lab.l));
    ui->pixel_lab_a_label->setText(QString().sprintf("%.2f", lab.a));
    ui->pixel_lab_b_label->setText(QString().sprintf("%.2f", lab.b));
}
void MainWindow::reset_sidemenu_pixel()
{
    ui->pixel_x_lineEdit->setText("-");
    ui->pixel_y_lineEdit->setText("-");
    ui->pixel_rgb_r_label->setText("-");
    ui->pixel_rgb_g_label->setText("-");
    ui->pixel_rgb_b_label->setText("-");
    ui->pixel_hsv_h_label->setText("-");
    ui->pixel_hsv_s_label->setText("-");
    ui->pixel_hsv_v_label->setText("-");
    ui->pixel_lab_l_label->setText("-");
    ui->pixel_lab_a_label->setText("-");
    ui->pixel_lab_b_label->setText("-");
}

void MainWindow::try_pick_pixel()
{
    bool ok = true;
    int x = ui->pixel_x_lineEdit->text().toInt(&ok);
    if (!ok || !is_correct_x(image, x))
    {
        if (is_pixel_picked){
            ui->pixel_x_lineEdit->setText(QString::number(pixel_point.x()));
            ui->pixel_y_lineEdit->setText(QString::number(pixel_point.y()));
        }
        return;
    }
    ok = true;
    int y = ui->pixel_y_lineEdit->text().toInt(&ok);
    if (!ok || !is_correct_y(image, y))
    {
        if (is_pixel_picked){
            ui->pixel_x_lineEdit->setText(QString::number(pixel_point.x()));
            ui->pixel_y_lineEdit->setText(QString::number(pixel_point.y()));
        }
        return;
    }
    QPoint point = QPoint(x, y);
    pick_pixel(point);
}

void MainWindow::on_actionOpen_triggered()
{
    load_image(QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath()));
}
void MainWindow::on_actionL_histogram_triggered()
{
    ImageDialog* p = new ImageDialog(this);
    p->setAttribute(Qt::WA_DeleteOnClose);
    p->set_content(HistogramMaker::histogram(500, 5, 1, 1, QColor(Qt::GlobalColor::black), QColor(Qt::GlobalColor::cyan), QColor(Qt::GlobalColor::darkCyan), HistogramMaker::image_l_values(image), 100, 0, 100));
    p->set_title("L-histogram");
    p->show();
}

void MainWindow::on_actionHSV_Transform_triggered()
{
    HSVTransformDialog* p = new HSVTransformDialog(this);
    p->set_imageEditor(this);
    p->show();
    ui->menuEdit->setEnabled(false);
}
void MainWindow::on_actionFilters_triggered()
{
    FiltersDialog* p = new FiltersDialog(this);
    p->set_imageEditor(this);
    p->show();
    ui->menuEdit->setEnabled(false);
}

void MainWindow::on_pixel_x_lineEdit_editingFinished()
{
    try_pick_pixel();
}
void MainWindow::on_pixel_y_lineEdit_editingFinished()
{
    try_pick_pixel();
}

