#include "tabledialog.h"
#include "ui_tabledialog.h"
#include <iostream>

TableDialog::TableDialog(QWidget *parent):
    QDialog(parent),
    ui(new Ui::TableDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setShowGrid(true);
}

void TableDialog::set_title(QString title)
{
    setWindowTitle(title);
}
void TableDialog::set_content(std::vector<QString> rows_headers, std::vector<QString> cols_headers, std::vector<std::vector<QString>> cells)
{
    int row_size = rows_headers.size();
    int col_size = cols_headers.size();

    if (cells.size() != row_size)
    {
        return;
    }
    for (int i = 0; i < cells.size(); i++)
    {
        if (cells[i].size() != col_size)
        {
            return;
        }
    }

    ui->tableWidget->setRowCount(row_size);
    ui->tableWidget->setColumnCount(col_size);
    for (int i = 0; i < row_size; i++)
    {
        ui->tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(rows_headers[i]));
    }
    for (int j = 0; j < col_size; j++)
    {
        ui->tableWidget->setHorizontalHeaderItem(j, new QTableWidgetItem(cols_headers[j]));
    }
    for (int i = 0; i < row_size; i++)
    {
        for (int j = 0; j < col_size; j++)
        {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(cells[i][j]));
        }
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->resizeRowsToContents();
}

TableDialog::~TableDialog()
{
    delete ui;
}
