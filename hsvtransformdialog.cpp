#include "hsvtransformdialog.h"
#include "ui_hsvtransformdialog.h"
#include <iostream>

HSVTransformDialog::HSVTransformDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HSVTransformDialog)
{
    ui->setupUi(this);
    setWindowTitle("HSV transform");
    reset();
}

HSVTransformDialog::~HSVTransformDialog()
{
    delete ui;
    image_editor = nullptr;
}

void HSVTransformDialog::reset()
{
    // h_slider
    ui->h_slider->setMinimum(0);
    ui->h_slider->setMaximum(h_def_values_size - 1);
    ui->h_slider->setSingleStep(1);
    ui->h_slider->setValue(0);
    ui->h_lineEdit->setText(QString::number(0));
    // s_slider
    ui->s_slider->setMinimum(0);
    ui->s_slider->setMaximum(s_def_values_size - 1);
    ui->s_slider->setSingleStep(1);
    int s_start = s_def_values_size / 2;
    ui->s_slider->setValue(s_start);
    ui->s_lineEdit->setText(QString::number(s_def_values[s_start]));
    // v_slider
    ui->v_slider->setMinimum(0);
    ui->v_slider->setMaximum(v_def_values_size - 1);
    ui->v_slider->setSingleStep(1);
    int v_start = v_def_values_size / 2;
    ui->v_slider->setValue(v_start);
    ui->v_lineEdit->setText(QString::number(v_def_values[v_start]));
}

void HSVTransformDialog::set_imageEditor(ImageEditor* image_editor)
{
    this->image_editor = image_editor;
}

void HSVTransformDialog::on_h_slider_sliderMoved(int position)
{
    ui->h_lineEdit->setText(QString::number(position));
    image_editor->try_hsv_transform(position, s_mul, v_mul);
    h_shift = position;
}
void HSVTransformDialog::on_s_slider_sliderMoved(int position)
{
    if (position < 0)
    {
        position = 0;
    }
    if (position >= s_def_values_size)
    {
        position = s_def_values_size - 1;
    }
    float value = s_def_values[position];
    ui->s_lineEdit->setText(QString::number(value));
    image_editor->try_hsv_transform(h_shift, value, v_mul);
    s_mul = value;
}
void HSVTransformDialog::on_v_slider_sliderMoved(int position)
{
    if (position < 0)
    {
        position = 0;
    }
    if (position >= v_def_values_size)
    {
        position = v_def_values_size - 1;
    }
    float value = v_def_values[position];
    ui->v_lineEdit->setText(QString::number(value));
    image_editor->try_hsv_transform(h_shift, s_mul, value);
    v_mul = value;
}
void HSVTransformDialog::on_h_lineEdit_editingFinished()
{
    bool ok = true;
    float value = ui->h_lineEdit->text().toFloat(&ok);
    if (!ok)
    {
        return;
    }
    while (value >= 360.f)
    {
        value -= 360.f;
    }
    while (value < 0)
    {
        value += 360.f;
    }
    ui->h_slider->setValue(int(value + 0.5));
    ui->h_lineEdit->setText(QString::number(value));
    image_editor->try_hsv_transform(value, s_mul, v_mul);
    h_shift = value;
}
void HSVTransformDialog::on_s_lineEdit_editingFinished()
{
    bool ok = true;
    float value = ui->s_lineEdit->text().toFloat(&ok);
    if (!ok)
    {
        return;
    }
    for (int i = 0; i < s_def_values_size; i++)
    {
        if (value <= s_def_values[i])
        {
            ui->s_slider->setValue(i);
            image_editor->hsv_transform(h_shift, value, v_mul);
            s_mul = value;
            return;
        }
    }
    ui->s_slider->setValue(s_def_values_size - 1);
    image_editor->try_hsv_transform(h_shift, value, v_mul);
    s_mul = value;
}
void HSVTransformDialog::on_v_lineEdit_editingFinished()
{
    bool ok = true;
    float value = ui->v_lineEdit->text().toFloat(&ok);
    if (!ok)
    {
        return;
    }
    for (int i = 0; i < v_def_values_size; i++)
    {
        if (value <= v_def_values[i])
        {
            ui->v_slider->setValue(i);
            image_editor->hsv_transform(h_shift, s_mul, value);
            v_mul = value;
            return;
        }
    }
    ui->v_slider->setValue(v_def_values_size - 1);
    image_editor->try_hsv_transform(h_shift, s_mul, value);
    v_mul = value;
}
void HSVTransformDialog::on_apply_button_clicked()
{
    image_editor->update_image();
    reset();
}
void HSVTransformDialog::on_reset_button_clicked()
{
    image_editor->reset_image();
    reset();
}

void HSVTransformDialog::on_HSVTransformDialog_finished(int result)
{
    image_editor->reset_image();
    image_editor->finished();
}
