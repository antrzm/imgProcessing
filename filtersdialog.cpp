#include "filtersdialog.h"
#include "ui_filtersdialog.h"


FiltersDialog::FiltersDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FiltersDialog)
{
    ui->setupUi(this);
    setWindowTitle("Filters");
    GAUSS_TAB_WIDGET_INDEX = ui->filters_tabWidget->indexOf(ui->gauss_parameters_tab);
    SOBEL_TAB_WIDGET_INDEX = ui->filters_tabWidget->indexOf(ui->sobel_parameters_tab);
    GABOR_TAB_WIDGET_INDEX = ui->filters_tabWidget->indexOf(ui->gabor_parameters_tab);
    CANNY_TAB_WIDGET_INDEX = ui->filters_tabWidget->indexOf(ui->canny_parameters_tab);
}

FiltersDialog::~FiltersDialog()
{
    delete ui;
}

void FiltersDialog::set_imageEditor(ImageEditor* image_editor)
{
    this->image_editor = image_editor;
}

void FiltersDialog::on_try_button_clicked()
{
    if (ui->filters_tabWidget->currentIndex() == GAUSS_TAB_WIDGET_INDEX)
    {
        bool ok = true;
        int window = ui->gauss_window_comboBox->currentText().toInt(&ok);
        if (!ok)
        {
            return;
        }
        float sigma = ui->gauss_sigma_lineEdit->text().toFloat(&ok);
        if (!ok)
        {
            return;
        }
        image_editor->gauss(window, sigma);
        return;
    }
    if (ui->filters_tabWidget->currentIndex() == SOBEL_TAB_WIDGET_INDEX)
    {
        image_editor->sobel();
        return;
    }
    if (ui->filters_tabWidget->currentIndex() == GABOR_TAB_WIDGET_INDEX)
    {
        bool ok = true;
        int window = ui->gabor_window_comboBox->currentText().toInt(&ok);
        if (!ok)
        {
            return;
        }
        float tetta = ui->gabor_tetta_comboBox->currentText().toFloat(&ok);
        if (!ok)
        {
            return;
        }
        image_editor->gabor(window, tetta);
        return;
    }
    if (ui->filters_tabWidget->currentIndex() == CANNY_TAB_WIDGET_INDEX){
        image_editor->canny();
        return;
    }
}

void FiltersDialog::on_apply_button_clicked()
{
    image_editor->update_image();
}

void FiltersDialog::on_reset_button_clicked()
{
    image_editor->reset_image();
}

void FiltersDialog::on_FiltersDialog_finished(int result)
{
    image_editor->reset_image();
    image_editor->finished();
}
