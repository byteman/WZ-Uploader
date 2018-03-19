#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QDebug>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    listPorts();
    connect(&wz,SIGNAL(debugMessage(int,QString)),this,SLOT(debugMessage(int,QString)));
    connect(&wz,SIGNAL(updateResult(int,int,int)),this,SLOT(updateResult(int,int,int)));

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
bool MainWindow::getPort(QString strPort,int& iPort)
{
    bool ok = false;
    iPort = strPort.right(1).toInt(&ok);
    if(ok==false) return false;
}
void MainWindow::on_pushButton_clicked()
{
    QFileDialog dlg;
   // QString file = dlg.getOpenFileName(this,"选择文件","",tr("Upload Files (*.bin)"));
    QString file = dlg.getOpenFileName(this,"选择文件","","*");
    qDebug() << "file=" << file;
    ui->lineEdit->setText(file);

}

void MainWindow::on_pushButton_2_clicked()
{


        wz.startUpdate(ui->lineEdit->text());


}
void MainWindow::timerEvent(QTimerEvent *)
{
    listPorts();
}

void MainWindow::updateResult(int result, int pos, int total)
{
    qDebug() << result << pos << total;
}

void MainWindow::debugMessage(int level, QString msg)
{
    qDebug() << msg;
}
#include <QMessageBox>
void MainWindow::on_pushButton_3_clicked()
{
    int port = 0;
    if(getPort(ui->cbxPort->currentText(),port))
    {
        if(!wz.openPort(port,9600))
        {
            QMessageBox::information(this,"error","open failed");
            return;
        }
    }
}
