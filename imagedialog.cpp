#include "imagedialog.h"
#include "ui_imagedialog.h"
#include <QPixmap>

ImageDialog::ImageDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::ImageDialog)
{
    ui->setupUi(this);
}

ImageDialog::~ImageDialog()
{}

void ImageDialog::set_title(QString title)
{
    setWindowTitle(title);
}

void ImageDialog::set_content(QImage image)
{
    ui->label->setPixmap(QPixmap::fromImage(image));
}
