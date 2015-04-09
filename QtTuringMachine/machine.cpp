#include "machine.h"
#include <QtWidgets>

#define standby 0
#define leftstep -1
#define rightstep 1
#define green QColor(170, 255, 170)

machine::machine(QString _ABC, QTableWidget *_p_table, QTableWidget *_p_tape, QObject *parent)
    : alphabet(_ABC), p_table(_p_table), p_tape(_p_tape)
{
    actTapeItem = 0;
    actColumn = 0;
    actRow = 0;
    delay = 0;
}

QString machine::getAlphabet() const
{
    return alphabet;
}

bool machine::parsingWord(QString word, QChar &sym, short &move, int &NextState)
{
    if (!isValidTableItem(word)) return false;
    if (word.at(1) == '<')
        move = leftstep;
    else
    {
        if (word.at(1) == '>')
            move = rightstep;
        else
        {
            if (word.at(1) == '.')
                move = standby;
        }
    }
    sym = word.at(0);
    word.remove(0, 2);
    NextState = word.toInt();
    return true;
}

bool machine::DoStep()
{
    qDebug() << "DoStep:";
    qDebug() << "ActTapeItem: " << actTapeItem+1;

    QChar sym;
    short move = 0;
    int NextState = 0;

    p_tape->item(0, actTapeItem)->setBackgroundColor(Qt::white);
    p_table->item(actRow, actColumn)->setBackgroundColor(Qt::white);

    if(!this->parsingWord(p_table->item(actRow, actColumn)->text(), sym, move, NextState))
        return false;

    if(p_tape->item(0, actTapeItem)->text() != sym)
        p_tape->item(0, actTapeItem)->setText(sym);

    qDebug() << "Test values";
    if(     (actTapeItem+move >= p_tape->colorCount())
            or (actTapeItem+move < 0)
            or (NextState > p_table->columnCount())
            or (NextState < 1)
            )
        return false;
    qDebug() << "Test passed";

    QThread::msleep(delay);

    actTapeItem += move;
    actColumn = NextState - 1;
    actRow = alphabet.indexOf(p_tape->item(0, actTapeItem)->text().at(0));

    p_table->item(actRow, actColumn)->setBackgroundColor(green);
    p_tape->item(0, actTapeItem)->setBackgroundColor(green);

    qDebug() << "End Step \n";
    return true;
}


bool machine::checkABC(QString word) const
{
    if ((word.size() == 0) or (word.contains(" ")))
        return false;
    if(word.at(word.size()-1) != '_')
        return false;
    for(size_t i = 0; i < word.size(); ++i)
    {
        for(size_t j = i+1; j < word.size(); ++j)
            if (word.at(i) == word.at(j))
                return false;
    }
    return true;
}

bool machine::setABC(QString word)
{
    if (this->checkABC(word))
    {
        alphabet = word;
        return true;
    }
    return false;
}

bool machine::isValidTapeItem(QString word) const
{
    return((word.size() == 1) && (alphabet.contains(word)));
}

bool machine::isValidTableItem(QString word) const
{
    QRegExp reg(".\[<>.]\[0-9]+");
    return((reg.exactMatch(word)) && (alphabet.contains(word.at(0))));
}

void machine::stepSlot()
{
    this->DoStep();
}

void machine::setActTapeCell(int row, int column)
{
    p_table->item(actRow, actColumn)->setBackgroundColor(Qt::white);
    p_tape->item(0, actTapeItem)->setBackgroundColor(Qt::white);

    actTapeItem = column;

    actRow = alphabet.indexOf(p_tape->item(0, actTapeItem)->text().at(0));
    actColumn = 0;

    p_table->item(actRow, actColumn)->setBackgroundColor(green);
    p_tape->item(0, actTapeItem)->setBackgroundColor(green);

    qDebug() << "\n ActTapeItem: " << actTapeItem+1 << '\n';
}

void machine::setDelay(int _delay)
{
    delay = _delay;
}
