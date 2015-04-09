#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include "machine.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

private:
    Ui::MainWindow *ui;                             // указатель на объект интерфейса
    QTableWidget *p_table;                          // укатель на таблицу команд
    QTableWidget *p_tape;                           // указатель на таблицу ленты
    QString ABC;                                    // алфафит
    machine *mash;                                  // указатель на объект машины
    bool work;                                      // переменная, хранящая состояние покоя
    bool resizeTape(uint size);                     // метод, установливающий кол-во ячеек ленты
    void resizeTable(uint columns, QString abc);    // метод, установливающий размер таблицы
    bool setTableRows(uint rows);                   // метод, установливающий кол-во строк в таблице
    bool setTableColumns(uint columns);             // метод, установливающий кол-во столбцов в таблице
    void refreshTapeHeader();                       // метод, обновляющий шапку таблицы ленты
    void refreshHorisontalHeader();                 // метод, обновляющий шапку таблицы (горизонтальную)
    void refreshVerticalHeader();                   // метод, обновляющий шапку таблицы (вертикальную)
    bool checkTableItem(QString word, short &kind); // метод, проверяющий тип введённой команды (сокращёная или нет)
private slots:
    void ABCchanged(QString);                       // слот, принимающий новый алфавит
    void insertState();                             // слот, добавляющий новое состояние в таблицу
    void deleteState();                             // слот, удаляющий последнее состояние из таблицы
    void setTableItem(int, int);                    // слот, обрабатывающий введённую команду таблицы
    void setTapeItem(int, int);                     // слот, обрабатывающий введённую ячейку ленты
    void startSlot();                               // слот, запускающий машину
    void stopSlot();                                // слот, останавливающий машину
};

#endif // MAINWINDOW_H
