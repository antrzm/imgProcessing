#ifndef FILTERSDIALOG_H
#define FILTERSDIALOG_H
#include <QDialog>
#include "imageeditor.h"

namespace Ui {
class FiltersDialog;
}

class FiltersDialog : public QDialog {
    Q_OBJECT

public:
    explicit FiltersDialog(QWidget *parent = 0);
    ~FiltersDialog();
    void set_imageEditor(ImageEditor* image_editor);

private slots:
    void on_FiltersDialog_finished(int result);
    void on_try_button_clicked();
    void on_apply_button_clicked();
    void on_reset_button_clicked();
private:
    Ui::FiltersDialog *ui = nullptr;
    ImageEditor* image_editor = nullptr;
    int GAUSS_TAB_WIDGET_INDEX;
    int SOBEL_TAB_WIDGET_INDEX;
    int GABOR_TAB_WIDGET_INDEX;
    int CANNY_TAB_WIDGET_INDEX;
    int OTSU_TAB_WIDGET_INDEX;
};
#endif // FILTERSDIALOG_H
