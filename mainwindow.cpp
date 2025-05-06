#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <csvreader.h>
#include <csvwriter.h>
#include <jsonreader.h>
#include <abstractreader.h>
#include <coloredtablewidgetitem.h>
#include <QFileDialog>
#include <QColor>
#include <QWidget>
#include <QMessageBox>
#include <iostream>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui -> setupUi(this);

    ui -> tableOutput -> setColumnCount(4);
    QStringList headers;
    headers << "ID" << "Name" << "Type" << "HP";

    ui -> tableOutput -> setHorizontalHeaderLabels(headers);
    ui -> tableOutput -> horizontalHeader() -> setSectionResizeMode(QHeaderView::Stretch);

    QStringList options;
    options << "Diesel"
            << "Gasoline"
            << "Hybrid"
            << "Electric";

    ui -> typePick -> addItems(options);

    connect(ui -> searchButton, &QPushButton::clicked, this, &MainWindow::search);
    connect(ui -> addFileButton, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(ui -> saveFileButton, &QPushButton::clicked, this, &MainWindow::saveToCsv);
    connect(ui -> addCarButton, &QPushButton::clicked, this, &MainWindow::addCar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void getItemColors(const Car& c, QColor& textColor, QColor& bgColor) {
    textColor = Qt::black;

    if (c.type == "Electric") {
        bgColor = QColor(190, 210, 255);
        textColor = QColor(0, 0, 150);
    }

    else if (c.hp < 100) {
        bgColor = QColor(255, 180, 180);
        textColor = QColor(150, 0, 0);
    }

    else if (c.hp > 250) {
        bgColor = QColor(180, 255, 180);
        textColor = QColor(0, 100, 0);
    }

    else {
        bgColor = QColor(255, 255, 190);
        textColor = QColor(100, 100, 0);
    }
}

void MainWindow::search()
{
    this -> updateTable();

    auto request = ui -> editSearch -> text();
    if (!request.isEmpty()) {
        int rowsCounter = 0;

        std::vector<Car> searchResults;
        for (const Car& c : cars) {
            QString carName = QString::fromStdString(c.name).toLower();
            QString carType = QString::fromStdString(c.type).toLower();
            QString searchLower = request.toLower();

            if (carName.contains(searchLower) || carType.contains(searchLower)) {
                searchResults.push_back(c);
            }
        }

        ui -> tableOutput -> setRowCount(searchResults.size());
        for (size_t i = 0; i < searchResults.size(); ++i) {
            const Car& c = searchResults[i];
            QColor txtColor, bgColor;
            getItemColors(c, txtColor, bgColor);

            ColoredTableWidgetItem *idItem = new ColoredTableWidgetItem(QString::number(c.id), txtColor, bgColor);
            ColoredTableWidgetItem *nameItem = new ColoredTableWidgetItem(QString::fromStdString(c.name), txtColor, bgColor);
            ColoredTableWidgetItem *typeItem = new ColoredTableWidgetItem(QString::fromStdString(c.type), txtColor, bgColor);
            ColoredTableWidgetItem *hpItem = new ColoredTableWidgetItem(QString::number(c.hp), txtColor, bgColor);

            ui -> tableOutput -> setItem(i, 0, idItem);
            ui -> tableOutput -> setItem(i, 1, nameItem);
            ui -> tableOutput -> setItem(i, 2, typeItem);
            ui -> tableOutput -> setItem(i, 3, hpItem);
        }


        ui -> tableOutput -> setRowCount(rowsCounter);
    }

    ui -> editSearch -> clear();

}

void MainWindow::openFile() {

    QString qFileName = QFileDialog::getOpenFileName(this, tr("Open Data File"), "", tr("All Files (*);;CSV Files (*.csv);;JSON Files (*.json)"));

    if (!qFileName.isEmpty()) {
        std::string newFileName = qFileName.toStdString();
        uploadTable(newFileName);
    }
}

bool tableSort(const Car& a, const Car& b) {

    if (a.hp != b.hp) {
        return a.hp > b.hp;
    }

    return a.name < b.name;

}

void MainWindow::uploadTable(const std::string& fileToLoad) {
    AbstractReader* reader = nullptr;
    bool isCsv = false;
    bool isJson = false;

    std::string lowerFile = fileToLoad;

    if (lowerFile.length() >= 4 && lowerFile.substr(lowerFile.length() - 4) == ".csv") {
        isCsv = true;
    } else if (lowerFile.length() >= 5 && lowerFile.substr(lowerFile.length() - 5) == ".json") {
        isJson = true;
    }

    if (isCsv) {
        reader = new CsvReader(fileToLoad);
    } else if (isJson) {
        reader = new JSONReader(fileToLoad);
    }

    if (reader) {
        cars = reader->readAll();
    }

    fileName = fileToLoad;

    updateTable();
}

void MainWindow::updateTable()
{
    std::sort(cars.begin(), cars.end(), tableSort);
    ui -> tableOutput -> setRowCount(cars.size());
    for (size_t i = 0; i < cars.size(); i++) {

        const Car& c = cars[i];

        QColor txtColor, bgColor;
        getItemColors(c, txtColor, bgColor);

        ColoredTableWidgetItem *idItem = new ColoredTableWidgetItem(QString::number(c.id), txtColor, bgColor);
        ColoredTableWidgetItem *nameItem = new ColoredTableWidgetItem(QString::fromStdString(c.name), txtColor, bgColor);
        ColoredTableWidgetItem *typeItem = new ColoredTableWidgetItem(QString::fromStdString(c.type), txtColor, bgColor);
        ColoredTableWidgetItem *hpItem = new ColoredTableWidgetItem(QString::number(c.hp), txtColor, bgColor);

        ui -> tableOutput -> setItem(i, 0, idItem);
        ui -> tableOutput -> setItem(i, 1, nameItem);
        ui -> tableOutput -> setItem(i, 2, typeItem);
        ui -> tableOutput -> setItem(i, 3, hpItem);

    }
}

void MainWindow::addCar()
{
    Car new_car;

    if     (!( ui -> nameEdit -> text()         .isEmpty()
            || ui -> typePick -> currentText()  .isEmpty()
            || ui -> hpEdit -> text()           .isEmpty()))
    {
        new_car.id    = cars.size()   +                              1;
        new_car.name = ui -> nameEdit -> text()         .toStdString();
        new_car.type = ui -> typePick -> currentText()  .toStdString();
        new_car.hp  = ui ->  hpEdit -> text()                 .toInt();
        cars.push_back(new_car);

    }

    this -> updateTable();
    ui -> nameEdit -> clear();
    ui -> hpEdit -> clear();
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
