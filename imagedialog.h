#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include <QDialog>
#include <QString>
#include <QImage>

namespace Ui
{
class ImageDialog;
}

class ImageDialog: public QDialog
{
    Q_OBJECT

private:
    Ui::ImageDialog *ui;
public:
    ImageDialog(QWidget *parent = 0);
    ~ImageDialog();
    void set_title(QString title);
    void set_content(QImage image);
};

#endif // IMAGEDIALOG_H
