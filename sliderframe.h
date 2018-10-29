#ifndef SLIDERFRAME_H
#define SLIDERFRAME_H

#include <QFrame>

namespace Ui {
class SliderFrame;
}

class SliderFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SliderFrame(QWidget *parent = 0);
    ~SliderFrame();

private slots:
    void on_horizontalSlider_sliderMoved(int position);

    void on_lineEdit_editingFinished();

private:
    Ui::SliderFrame *ui;
};

#endif // SLIDERFRAME_H
