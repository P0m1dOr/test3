#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cars.h"
#include "abstractreader.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void search();
    void openFile();
    void uploadTable(const std::string& fileToLoad);
    void updateTable();
    void addCar();
    void saveToCsv();

private:
    Ui::MainWindow *ui;
    std::string fileName;
    std::vector<Car> cars;
};
#endif // MAINWINDOW_H
