#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <csvreader.h>
#include <csvwriter.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    ui -> tableOutput -> setColumnCount(4);
    QStringList headers;
    headers << "ID" << "Model" << "Type" << "HP";

    ui -> tableOutput -> setHorizontalHeaderLabels(headers);
    ui -> tableOutput -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

    QStringList options;
    options << "Diesel"
            << "Gasoline"
            << "Hybrid"
            << "Electro";

    ui -> typePick -> addItems(options);

    connect(ui -> searchButton, &QPushButton::clicked, this, &MainWindow::search);
    connect(ui -> addFileButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui -> saveFileButton, &QPushButton::clicked, this, &MainWindow::saveToCsv);
    connect(ui -> addCarButton, &QPushButton::clicked, this, &MainWindow::addCar);


    CsvReader csv("cars.csv");
    if (csv.isOpen()) {
        cars = csv.readAll();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::search()
{
  this -> updateTable();

  auto request = ui -> editSearch -> text();
  if (request != "") {
      int rowsCounter = 0;

      for (size_t i = 0; i < cars.size(); i++) {
          const Car& c = cars[i];

          if (c.name.find(request.toStdString()) != std::string::npos || c.type.find(request.toStdString()) != std::string::npos) {
              QTableWidgetItem *id = new QTableWidgetItem(QString::number(c.id));
              QTableWidgetItem *name = new QTableWidgetItem(QString::fromStdString(c.name));
              QTableWidgetItem *type = new QTableWidgetItem(QString::fromStdString(c.type));
              QTableWidgetItem *studio = new QTableWidgetItem(QString::number(c.hp));

              ui -> tableOutput -> setItem(rowsCounter, 0, id);
              ui -> tableOutput -> setItem(rowsCounter, 1, name);
              ui -> tableOutput -> setItem(rowsCounter, 2, type);
              ui -> tableOutput -> setItem(rowsCounter, 3, studio);

              rowsCounter += 1;
          }
      }

      ui -> tableOutput -> setRowCount(rowsCounter);

  }

    ui -> editSearch -> clear();

}

void MainWindow::openFile() {

    QString name = QFileDialog::getOpenFileName(this, tr("Open CSV File"), "", tr("CSV Files (*.csv)"));

    if (!name.isEmpty()) {

        fileName = name.toStdString();
        this -> uploadTable();
    }

}

bool tableSort(const Car& a, const Car& b) {

    if (a.hp > b.hp) {
        return true;
    }

    else if (a.hp == b.hp) {
        return a.name < b.name;
    }

    else {
      return false;
    }

}

void MainWindow::uploadTable() {

    CsvReader csv(fileName);

    if (csv.isOpen()) {

        cars = csv.readAll();

        std::sort(cars.begin(), cars.end(), tableSort);
        ui -> tableOutput -> setRowCount(cars.size());

        for (size_t i = 0; i < cars.size(); i++) {

            const Car& c = cars[i];
            QTableWidgetItem *id = new QTableWidgetItem(QString::number(c.id));
            QTableWidgetItem *name = new QTableWidgetItem(QString::fromStdString(c.name));
            QTableWidgetItem *type = new QTableWidgetItem(QString::fromStdString(c.type));
            QTableWidgetItem *hp = new QTableWidgetItem(QString::number(c.hp));

            ui -> tableOutput -> setItem(i, 0, id);
            ui -> tableOutput -> setItem(i, 1, name);
            ui -> tableOutput -> setItem(i, 2, type);
            ui -> tableOutput -> setItem(i, 3, hp);

        }
    }
}

void MainWindow::updateTable()
{
        std::sort(cars.begin(), cars.end(), tableSort);
        ui -> tableOutput -> setRowCount(cars.size());
        for (size_t i = 0; i < cars.size(); i++) {

            const Car& c = cars[i];
            QTableWidgetItem *id = new QTableWidgetItem(QString::number(c.id));
            QTableWidgetItem *name = new QTableWidgetItem(QString::fromStdString(c.name));
            QTableWidgetItem *type = new QTableWidgetItem(QString::fromStdString(c.type));
            QTableWidgetItem *hp = new QTableWidgetItem(QString::number(c.hp));

            ui -> tableOutput -> setItem(i, 0, id);
            ui -> tableOutput -> setItem(i, 1, name);
            ui -> tableOutput -> setItem(i, 2, type);
            ui -> tableOutput -> setItem(i, 3, hp);

        }
}

void MainWindow::addCar()
{
    Car new_car;

    if     (!( ui -> nameEdit -> text()         .isEmpty()
            || ui -> typePick -> currentText()  .isEmpty()
            || ui -> hpEdit -> text()           .isEmpty()))
            {
                    new_car.id = cars.size() + 1;
                    new_car.name = ui -> nameEdit -> text().toStdString();
                    new_car.type = ui -> typePick -> currentText().toStdString();
                    new_car.hp = ui -> hpEdit -> text().toInt();
                    cars.push_back(new_car);
            }

    this -> updateTable();
}

void MainWindow::saveToCsv()
{
    QString saveName = QFileDialog::getSaveFileName(this, "Save to CSV", "", "CSV Files (*.csv)");
    if (!saveName.isEmpty()) {
        std::string name = saveName.toLatin1().data();
        CsvWriter writer(name);
        writer.writeAll(cars);
    }
}
