#include "coloredtablewidgetitem.h"

ColoredTableWidgetItem::ColoredTableWidgetItem(const QString &text, const QColor &textColor, const QColor &backgroundColor) : QTableWidgetItem(text)
{
    setForeground(QBrush(textColor));
    setBackground(QBrush(backgroundColor));
}

