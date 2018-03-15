#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    listPorts();
}
void MainWindow::listPorts()
{
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();

    QSerialPortInfo port;
    foreach(port,ports){
        ui->cbxPort->addItem(port.portName());
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
