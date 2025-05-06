#ifndef COLOREDTABLEWIDGETITEM_H
#define COLOREDTABLEWIDGETITEM_H

#include <QTableWidgetItem>
#include <QColor>
#include <QBrush>
#include "cars.h"

class ColoredTableWidgetItem : public QTableWidgetItem
{
public:
    ColoredTableWidgetItem(const QString &text, const QColor &textColor = Qt::black, const QColor &backgroundColor = Qt::white);

};

#endif // COLOREDTABLEWIDGETITEM_H
