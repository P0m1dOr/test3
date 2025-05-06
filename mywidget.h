#ifndef MYWIDGET_H
#define MYWIDGET_H
#include <QFileDialog>
#include <QColor>
#include <QWidget>
#include <QTableWidgetItem>
#include <QTableView>
#include <QHeaderView>
#include <QPalette>

class MyWidget : public QTableWidgetItem  {
public:
    explicit MyWidget(QWidget *parent = nullptr) : QTableWidgetItem(parent) {}
    // void setTextColor(QColor color);
};
#endif // MYWIDGET_H
