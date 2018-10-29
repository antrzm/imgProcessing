#include "sliderframe.h"
#include "ui_sliderframe.h"

SliderFrame::SliderFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SliderFrame)
{
    ui->setupUi(this);
}

SliderFrame::~SliderFrame()
{
    delete ui;
}

void SliderFrame::on_horizontalSlider_sliderMoved(int position)
{

}

void SliderFrame::on_lineEdit_editingFinished()
{}
