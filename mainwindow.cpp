#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "machine.h"
#include <QtWidgets>

#define delay 200
#define TapeSize 100

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    p_table = ui->Table;
    p_tape = ui->Tape;

    ABC = "01_";
    ui->ABC_line->setText(ABC);

    mash = new machine(ABC, p_table, p_tape);

    this->resizeTable(2, ABC);

    p_table->item(0, 0)->setText("0>1");
    p_table->item(1, 0)->setText("1>1");
    p_table->item(2, 0)->setText("_>1");

    p_tape->setRowCount(1);

    this->resizeTape(TapeSize);

    this->setWindowTitle("Эмулятор Машины Тьюринга");

    p_table->setSelectionMode(QAbstractItemView::NoSelection);
    p_tape->setSelectionMode(QAbstractItemView::NoSelection);

    ui->delay_box->setValue(delay);
    mash->setDelay(delay);

    QObject::connect(ui->ABC_line, SIGNAL(textChanged(QString)), this, SLOT(ABCchanged(QString)));
    QObject::connect(ui->step, SIGNAL(clicked()), mash, SLOT(stepSlot()));
    QObject::connect(ui->new_column, SIGNAL(clicked()), this, SLOT(insertState()));
    QObject::connect(ui->del_column, SIGNAL(clicked()), this, SLOT(deleteState()));
    QObject::connect(p_table, SIGNAL(cellChanged(int, int)), this, SLOT(setTableItem(int, int)));
    QObject::connect(p_tape, SIGNAL(cellChanged(int, int)), this, SLOT(setTapeItem(int, int)));
    QObject::connect(p_tape, SIGNAL(cellClicked(int,int)), mash, SLOT(setActTapeCell(int, int)));
    QObject::connect(ui->delay_box, SIGNAL(valueChanged(int)), mash, SLOT(setDelay(int)));
    QObject::connect(ui->start, SIGNAL(clicked()), this, SLOT(startSlot()));
    QObject::connect(ui->stop, SIGNAL(clicked()), this, SLOT(stopSlot()));

}


bool MainWindow::resizeTape(uint size)
{
    if (p_tape->columnCount() < size)
    {
        int b_count = p_tape->columnCount();
        p_tape->setColumnCount(size);
        for(; b_count < size; ++b_count)
        {
            p_tape->setItem(0, b_count, new QTableWidgetItem(QString("_")));
        }
    }
    if (p_tape->columnCount() > size)
    {
        for(;p_tape->columnCount() > size;)
            p_tape->removeColumn(p_tape->columnCount()-1);
    }
    if (p_tape->columnCount() == size)
    {
        refreshTapeHeader();
        return true;
    }
    else
        return false;
}

void MainWindow::refreshTapeHeader()
{
    QStringList *Lst = new QStringList;
    for(int i = 0; i < p_tape->columnCount(); ++i)
        *Lst << QString::number(i+1);
    p_tape->setHorizontalHeaderLabels(*Lst);
}

void MainWindow::refreshHorisontalHeader()
{
    QStringList *Lst = new QStringList;
    for(int i = 0; i < p_table->columnCount(); ++i)
        *Lst << QString("q " + QString::number(i+1));
    p_table->setHorizontalHeaderLabels(*Lst);
}

void MainWindow::refreshVerticalHeader()
{
    QStringList *Lst = new QStringList;
    for(int i = 0; i < p_table->rowCount(); ++i)
        *Lst << QString(ABC.at(i));
    p_table->setVerticalHeaderLabels(*Lst);
}

bool MainWindow::setTableRows(uint rows)
{
    if (p_table->rowCount() < rows)
    {
        int row_count = p_table->rowCount();
        p_table->setRowCount(rows);
        for(; row_count < rows; ++row_count)
            for(int t_col = 0; t_col < p_table->rowCount(); ++t_col)
                p_table->setItem(row_count, t_col, new QTableWidgetItem(QString("_")));
    }
    if (p_table->rowCount() > rows)
    {
        for(;p_table->rowCount() > rows;)
            p_table->removeRow(p_table->rowCount()-1);
    }
    if (p_table->rowCount() != rows)
        return false;
    this->refreshVerticalHeader();
    return true;
}

bool MainWindow::setTableColumns(uint columns)
{

    if (p_table->columnCount() < columns)
    {
        int col_count = p_table->columnCount();
        p_table->setColumnCount(columns);
        for(; col_count < columns; ++col_count)
            for(int t_row = 0; t_row < p_table->rowCount(); ++t_row)
                p_table->setItem(t_row, col_count, new QTableWidgetItem(QString("_")));
    }
    if (p_table->columnCount() > columns)
    {
        for(;p_table->columnCount() > columns;)
            p_table->removeColumn(p_table->columnCount()-1);
    }
    if (p_table->columnCount() != columns)
        return false;
    this->refreshHorisontalHeader();
    return true;
}



void MainWindow::resizeTable(uint columns, QString abc)
{
    this->setTableColumns(columns);
    this->setTableRows(abc.size());
    this->refreshHorisontalHeader();
    this->refreshVerticalHeader();
}

void MainWindow::ABCchanged(QString word)
{
    qDebug() << "ABC:";
    if(mash->checkABC(word))
    {
        qDebug() << 1;
        mash->alphabet = ABC = word;
        qDebug() << 2;
        this->setTableRows(ABC.size());
        qDebug() << 3;

    }
    else
    {
        qDebug() << 4;
        ui->ABC_line->setText(ABC);
    }
}

void MainWindow::insertState()
{
    this->setTableColumns(p_table->columnCount()+1);
}

void MainWindow::deleteState()
{
    if(p_table->columnCount() > 0)
        p_table->removeColumn(p_table->columnCount() - 1);
}

bool MainWindow::checkTableItem(QString word, short &kind)
{
    QRegExp reg(".\[<>.]\[0-9]+");
    if (reg.exactMatch(word) && ABC.contains(word.at(0)))
    {
        kind = 3;
        return true;
    }

    reg = QRegExp(".\[<>.]");
    if (reg.exactMatch(word) && ABC.contains(word.at(0)))
    {
        kind = 2;
        return true;
    }

    reg = QRegExp("[<>.]\[0-9]+");
    if (reg.exactMatch(word))
    {
        kind = 1;
        return true;
    }

    reg = QRegExp("[<>.]");
    if (reg.exactMatch(word))
    {
        kind = 0;
        return true;
    }

    qDebug() << "not valid item";
    kind = -1;
    return false;
}

void MainWindow::setTableItem(int row, int column)
{
    short kind = 0;
    if(!this->checkTableItem(p_table->item(row, column)->text(), kind)
       or ((kind < 0) or (kind > 3))
                                    )
    {
        p_table->item(row, column)->setText("_");
        qDebug() << "TableItem Error";
        return;
    }


    if (kind == 0)
    {
        QString word;
        word = ABC.at(row);
        word += p_table->item(row, column)->text();
        word += QString::number(column+1);
        p_table->item(row, column)->setText(word);
    }

    if (kind == 1)
    {
        QString word;
        word = ABC.at(row);
        word += p_table->item(row, column)->text();
        p_table->item(row, column)->setText(word);
    }

    if (kind == 2)
    {
        QString word;
        word += p_table->item(row, column)->text();
        word += QString::number(column+1);
        p_table->item(row, column)->setText(word);
    }
}

void MainWindow::setTapeItem(int row, int column)
{
    if(!mash->isValidTapeItem( p_tape->item(0, column)->text() ))
        p_tape->item(0, column)->setText("_");
}



void MainWindow::startSlot()
{
    ui->start->setDisabled(true);
    ui->stop->setDisabled(false);
    work = true;
    while (work)
    {
        if(!mash->DoStep())
        {
            this->stopSlot();
            work = false;
        }
        QCoreApplication::processEvents();
    }
}

void MainWindow::stopSlot()
{
    work = false;

    ui->start->setDisabled(false);
    ui->stop->setDisabled(true);

    QMessageBox msgBox;
    msgBox.setText("Выполнение завершено");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Сообщение");
    msgBox.exec();
}
