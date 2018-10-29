#ifndef TABLEDIALOG_H
#define TABLEDIALOG_H

#include <QDialog>

namespace Ui
{
class TableDialog;
}

class TableDialog: public QDialog
{
    Q_OBJECT

private:
    Ui::TableDialog *ui;
public:
    TableDialog(QWidget *parent = 0);
    ~TableDialog();
    void set_title(QString title);
    void set_content(std::vector<QString> rows_headers, std::vector<QString> cols_headers, std::vector<std::vector<QString>> cells);
};

#endif // TABLEDIALOG_H
