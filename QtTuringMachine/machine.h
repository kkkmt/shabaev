#ifndef MACHINE_H
#define MACHINE_H

#include <QObject>
#include <QtWidgets>


class machine : public QObject
{
    Q_OBJECT
public:
    machine(QString ABC, QTableWidget *_p_table, QTableWidget *_p_tape, QObject *parent = 0); // конструктор класса
    QString alphabet;                               // алфафит
    QString getAlphabet() const;                    // метод возвращает алфавит
    bool isValidTapeItem(QString word) const;       // метод проверяет, является ли значение ячейки ленты допуcтимым
    bool isValidTableItem(QString word) const;      // метод проверяет, является ли команда допуcтимой
    bool checkABC(QString word) const;              // метод проверяет, является строка допустимым алфавитом
    bool setABC(QString word);                      // метод устанавливает алфавит
    bool DoStep();                                  // выполнить один шаг

private:
    QTableWidget *p_table;                          // укатель на таблицу команд
    QTableWidget *p_tape;                           // указатель на таблицу ленты
    uint actTapeItem;                               // переменная, хранящая номер активной ячейки
    int actRow;                                     // переменная, хранящая номер строки активной ячейки таблицы
    int actColumn;                                  // переменная, хранящая номер столбца активной ячейки таблицы
    int delay;                                      // переменная, хранящая значение задержки
    bool parsingWord(QString word, QChar &sym, short &move, int &NextState); // метод парсинга команды таблицы


public slots:
    void stepSlot();                                // слот "сделать шаг"
    void setActTapeCell(int, int);                  // слот, принимающий индекс текущей ячейки ленты
    void setDelay(int _delay);                      // слот, принимающий значение задержки
};

#endif // MACHINE_H
