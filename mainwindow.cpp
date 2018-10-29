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
    turn_off_menubar_image();
    turn_off_menubar_edit();
    turn_off_sidemenu_pixel();
    turn_off_sidemenu_rectangle();
    reset_pixel();
    reset_rectangle();
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
    reset_rectangle();
    // menubar
    turn_on_menubar_image();
    turn_on_menubar_edit();
    // sidemenu
    turn_on_sidemenu_pixel();
    turn_on_sidemenu_rectangle();
    mutex.unlock();
}
void MainWindow::update_image()
{
    mutex.lock();
    preserved_image = QImage();
    reset_pixel();
    reset_rectangle();
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
/* Rectangle */
void MainWindow::pick_rectangle(QPoint point_0, QPoint point_1)
{
    if (image.isNull())
    {
        return;
    }
    if (!is_point_correct(image, point_0))
    {
        return;
    }
    if (!is_point_correct(image, point_1))
    {
        return;
    }
    is_rectangle_picked = true;
    rectangle_point_0 = point_0;
    rectangle_point_1 = point_1;
    set_sidemenu_rectangle(rectangle_point_0, rectangle_point_1);
    turn_on_sidemenu_rectangle_rgb_hsv_lab_buttons();
}
void MainWindow::reset_rectangle()
{
    is_rectangle_picked = false;
    rectangle_point_0 = QPoint();
    rectangle_point_1 = QPoint();
    reset_sidemenu_rectangle();
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
/* 2.2. Rectangle */
void MainWindow::turn_on_sidemenu_rectangle()
{
    ui->rectangle_groupBox->setEnabled(true);
    ui->rectangle_rgb_button->setEnabled(false);
    ui->rectangle_hsv_button->setEnabled(false);
    ui->rectangle_lab_button->setEnabled(false);
}
void MainWindow::turn_on_sidemenu_rectangle_rgb_hsv_lab_buttons()
{
    ui->rectangle_rgb_button->setEnabled(true);
    ui->rectangle_hsv_button->setEnabled(true);
    ui->rectangle_lab_button->setEnabled(true);
}
void MainWindow::turn_off_sidemenu_rectangle()
{
    ui->rectangle_groupBox->setEnabled(false);
}
void MainWindow::set_sidemenu_rectangle(QPoint point_0, QPoint point_1)
{
    ui->rectangle_x0_lineEdit->setText(QString::number(point_0.x()));
    ui->rectangle_y0_lineEdit->setText(QString::number(point_0.y()));
    ui->rectangle_x1_lineEdit->setText(QString::number(point_1.x()));
    ui->rectangle_y1_lineEdit->setText(QString::number(point_1.y()));
}
void MainWindow::reset_sidemenu_rectangle()
{
    ui->rectangle_x0_lineEdit->setText("-");
    ui->rectangle_y0_lineEdit->setText("-");
    ui->rectangle_x1_lineEdit->setText("-");
    ui->rectangle_y1_lineEdit->setText("-");
}
/* ---------- */




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
void MainWindow::try_pick_rectangle()
{
    bool ok = true;
    int x0 = ui->rectangle_x0_lineEdit->text().toInt(&ok);
    if (!ok || !is_correct_x(image, x0))
    {
        if (is_rectangle_picked){
            set_sidemenu_rectangle(rectangle_point_0, rectangle_point_1);
        }
        return;
    }
    ok = true;
    int y0 = ui->rectangle_y0_lineEdit->text().toInt(&ok);
    if (!ok || !is_correct_y(image, y0))
    {
        if (is_rectangle_picked){
            set_sidemenu_rectangle(rectangle_point_0, rectangle_point_1);
        }
        return;
    }
    ok = true;
    int x1 = ui->rectangle_x1_lineEdit->text().toInt(&ok);
    if (!ok || !is_correct_x(image, x1))
    {
        if (is_rectangle_picked){
            set_sidemenu_rectangle(rectangle_point_0, rectangle_point_1);
        }
        return;
    }
    ok = true;
    int y1 = ui->rectangle_y1_lineEdit->text().toInt(&ok);
    if (!ok || !is_correct_y(image, y1))
    {
        if (is_rectangle_picked)
        {
            set_sidemenu_rectangle(rectangle_point_0, rectangle_point_1);
        }
        return;
    }
    QPoint point_0 = QPoint(x0, y0);
    QPoint point_1 = QPoint(x1, y1);
    pick_rectangle(point_0, point_1);
}
void MainWindow::open_rectangle_rgb_dialog()
{
    mutex.lock();

    TableDialog* rectangle_rgb_dialog = new TableDialog(this);
    rectangle_rgb_dialog->setAttribute(Qt::WA_DeleteOnClose);
    std::vector<QString> rows_headers;
    std::vector<QString> cols_headers;
    std::vector<std::vector<QString>> cells;
    int x_min = (rectangle_point_0.x() < rectangle_point_1.x()) ? rectangle_point_0.x() :rectangle_point_1.x();
    int x_max = (rectangle_point_0.x() >= rectangle_point_1.x()) ? rectangle_point_0.x() :rectangle_point_1.x();
    int y_min = (rectangle_point_0.y() < rectangle_point_1.y()) ? rectangle_point_0.y() : rectangle_point_1.y();
    int y_max = (rectangle_point_0.y() >= rectangle_point_1.y()) ? rectangle_point_0.y() :rectangle_point_1.y();

    for (int y = y_min; y <= y_max; y++)
    {
        rows_headers.push_back(QString::number(y));
    }
    for (int x = x_min; x <= x_max; x++)
    {
        cols_headers.push_back(QString::number(x));
    }
    for (int y = y_min; y <= y_max; y++)
    {
        std::vector<QString> row;
        for (int x = x_min; x <= x_max; x++)
        {
            color::RGBA rgba;
            color::Tools::qcolor2rgba(QColor(image.pixelColor(x, y)), rgba);
            row.push_back(QString("(") + QString::number(rgba.r) + QString(", ") + QString::number(rgba.g) + QString(", ") + QString::number(rgba.b) + QString(")"));
        }
        cells.push_back(row);
    }

    rectangle_rgb_dialog->set_title(QString("RGB rectangle"));
    rectangle_rgb_dialog->set_content(rows_headers, cols_headers, cells);
    rectangle_rgb_dialog->show();

    mutex.unlock();
}
void MainWindow::open_rectangle_hsv_dialog()
{
    mutex.lock();

    TableDialog* rectangle_hsv_dialog = new TableDialog(this);
    rectangle_hsv_dialog->setAttribute(Qt::WA_DeleteOnClose);
    std::vector<QString> rows_headers;
    std::vector<QString> cols_headers;
    std::vector<std::vector<QString>> cells;
    int x_min = (rectangle_point_0.x() < rectangle_point_1.x()) ? rectangle_point_0.x() :rectangle_point_1.x();
    int x_max = (rectangle_point_0.x() >= rectangle_point_1.x()) ? rectangle_point_0.x() :rectangle_point_1.x();
    int y_min = (rectangle_point_0.y() < rectangle_point_1.y()) ? rectangle_point_0.y() : rectangle_point_1.y();
    int y_max = (rectangle_point_0.y() >= rectangle_point_1.y()) ? rectangle_point_0.y() :rectangle_point_1.y();

    for (int y = y_min; y <= y_max; y++)
    {
        rows_headers.push_back(QString::number(y));
    }
    for (int x = x_min; x <= x_max; x++)
    {
        cols_headers.push_back(QString::number(x));
    }
    for (int y = y_min; y <= y_max; y++)
    {
        std::vector<QString> row;
        for (int x = x_min; x <= x_max; x++)
        {
            color::RGBA rgba;
            color::Tools::qcolor2rgba(QColor(image.pixelColor(x, y)), rgba);
            color::HSV hsv;
            color::Converter::rgba2hsv(rgba, hsv);
            row.push_back(QString().sprintf("(%.2f, %.2f, %.2f)", hsv.h, hsv.s, hsv.v));
        }
        cells.push_back(row);
    }

    rectangle_hsv_dialog->set_title(QString("HSV rectangle"));
    rectangle_hsv_dialog->set_content(rows_headers, cols_headers, cells);
    rectangle_hsv_dialog->show();

    mutex.unlock();
}
void MainWindow::open_rectangle_lab_dialog()
{
    mutex.lock();

    TableDialog* rectangle_lab_dialog = new TableDialog(this);
    rectangle_lab_dialog->setAttribute(Qt::WA_DeleteOnClose);
    std::vector<QString> rows_headers;
    std::vector<QString> cols_headers;
    std::vector<std::vector<QString>> cells;
    int x_min = (rectangle_point_0.x() < rectangle_point_1.x()) ? rectangle_point_0.x() :rectangle_point_1.x();
    int x_max = (rectangle_point_0.x() >= rectangle_point_1.x()) ? rectangle_point_0.x() :rectangle_point_1.x();
    int y_min = (rectangle_point_0.y() < rectangle_point_1.y()) ? rectangle_point_0.y() : rectangle_point_1.y();
    int y_max = (rectangle_point_0.y() >= rectangle_point_1.y()) ? rectangle_point_0.y() :rectangle_point_1.y();

    for (int y = y_min; y <= y_max; y++)
    {
        rows_headers.push_back(QString::number(y));
    }
    for (int x = x_min; x <= x_max; x++)
    {
        cols_headers.push_back(QString::number(x));
    }
    for (int y = y_min; y <= y_max; y++)
    {
        std::vector<QString> row;
        for (int x = x_min; x <= x_max; x++)
        {
            color::RGBA rgba;
            color::Tools::qcolor2rgba(QColor(image.pixelColor(x, y)), rgba);
            color::LAB lab;
            color::Converter::rgba2lab(rgba, lab);
            row.push_back(QString().sprintf("(%.2f, %.2f, %.2f)", lab.l, lab.a, lab.b));
        }
        cells.push_back(row);
    }

    rectangle_lab_dialog->set_title(QString("LAB rectangle"));
    rectangle_lab_dialog->set_content(rows_headers, cols_headers, cells);
    rectangle_lab_dialog->show();

    mutex.unlock();
}
void MainWindow::on_actionOpen_triggered()
{
    load_image(QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath()));
}
void MainWindow::on_actionL_histogram_triggered()
{
    ImageDialog* p = new ImageDialog(this);
    p->setAttribute(Qt::WA_DeleteOnClose);
    p->set_content(HistogramMaker::histogram(500, 5, 1, 1, QColor(Qt::GlobalColor::gray), QColor(Qt::GlobalColor::black), QColor(Qt::GlobalColor::white), HistogramMaker::image_l_values(image), 100, 0, 100));
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
void MainWindow::on_rectangle_rgb_button_clicked()
{
    open_rectangle_rgb_dialog();
}
void MainWindow::on_rectangle_hsv_button_clicked()
{
    open_rectangle_hsv_dialog();
}
void MainWindow::on_rectangle_lab_button_clicked()
{
    open_rectangle_lab_dialog();
}
void MainWindow::on_pixel_x_lineEdit_editingFinished()
{
    try_pick_pixel();
}
void MainWindow::on_pixel_y_lineEdit_editingFinished()
{
    try_pick_pixel();
}
void MainWindow::on_rectangle_x0_lineEdit_editingFinished()
{
    try_pick_rectangle();
}
void MainWindow::on_rectangle_y0_lineEdit_editingFinished()
{
    try_pick_rectangle();
}
void MainWindow::on_rectangle_x1_lineEdit_editingFinished()
{
   try_pick_rectangle();
}
void MainWindow::on_rectangle_y1_lineEdit_editingFinished()
{
    try_pick_rectangle();
}
