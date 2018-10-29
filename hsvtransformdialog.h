#ifndef HSVTRANSFORMDIALOG_H
#define HSVTRANSFORMDIALOG_H

#include <QDialog>
#include "imageeditor.h"

namespace Ui {
class HSVTransformDialog;
}

class HSVTransformDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HSVTransformDialog(QWidget *parent = 0);
    ~HSVTransformDialog();
    void set_imageEditor(ImageEditor* image_editor);

private slots:
    void on_h_slider_sliderMoved(int position);
    void on_s_slider_sliderMoved(int position);
    void on_v_slider_sliderMoved(int position);
    void on_h_lineEdit_editingFinished();
    void on_s_lineEdit_editingFinished();
    void on_v_lineEdit_editingFinished();
    void on_apply_button_clicked();
    void on_reset_button_clicked();

    void on_HSVTransformDialog_finished(int result);

private:
    Ui::HSVTransformDialog *ui = nullptr;
    ImageEditor* image_editor = nullptr;
    static const int h_def_values_size = 360;
    static const int s_def_values_size = 7;
    static const int v_def_values_size = 7;
    float s_def_values[s_def_values_size] = {0.1f, 0.2f, 0.5f, 1.f, 2.f, 5.f, 10.f};
    float v_def_values[v_def_values_size] = {0.1f, 0.2f, 0.5f, 1.f, 2.f, 5.f, 10.f};
    float h_shift = 0.f;
    float s_mul = 1.f;
    float v_mul = 1.f;
    void reset();
};

#endif // HSVTRANSFORMDIALOG_H
