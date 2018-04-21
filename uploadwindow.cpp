#include "uploadwindow.h"
#include "ui_uploadwindow.h"
#include <QSerialPortInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDebug>
#include <QTranslator>
UploadWindow::UploadWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UploadWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    this->setWindowTitle(tr("WzUpdate"));
    this->setWindowIcon(QIcon(":/icon.ico"));
    listPorts();
    connect(&wz,SIGNAL(debugMessage(int,QString)),this,SLOT(debugMessage(int,QString)));
    connect(&wz,SIGNAL(updateResult(int,int,int)),this,SLOT(updateResult(int,int,int)));
    this->startTimer(1000);
}
void UploadWindow::listPorts()
{


    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();


    if(ports.size() == ui->cbxPorts->count())
    {
        return;
    }
    ui->cbxPorts->clear();
    QSerialPortInfo port;
    foreach(port,ports){

            ui->cbxPorts->addItem(port.portName());



    }
}
UploadWindow::~UploadWindow()
{
    delete ui;
}
bool UploadWindow::getPort(QString strPort,int& iPort)
{
    bool ok = false;
    iPort = strPort.right(1).toInt(&ok);
    if(ok==false) return false;
}
void UploadWindow::on_pushButton_3_clicked()
{
    int port = 0;
    if(getPort(ui->cbxPorts->currentText(),port))
    {
        if(!wz.openPort(port,9600))
        {
            QMessageBox::information(this,"error","open failed");
            return;
        }
    }
    ui->stackedWidget->setCurrentIndex(2);
}

void UploadWindow::on_pushButton_5_clicked()
{
    QFileDialog dlg;
    QString file = dlg.getOpenFileName(this,"选择文件","",tr("Upload Files (*.bin)"));
   // QString file = dlg.getOpenFileName(this,"选择文件","","*");
    qDebug() << "file=" << file;
    ui->lineEdit->setText(file);
}
void UploadWindow::showErrMessage(QString msg)
{
    QMessageBox::information(this,tr("msg_err_title"),msg);

}

void UploadWindow::showInfoMessage(QString msg)
{
    QMessageBox::information(this,tr("msg_info_title"),msg);

}

void UploadWindow::showStatusMessage(QString msg)
{
    ui->statusbar->showMessage(msg);
}
void UploadWindow::updateResult(int result, int pos, int total)
{
    //qDebug() << result << pos << total;
    //ui->progressBar->setMaximum(total);
    ui->btnUpdate->setEnabled(false);
    switch(result)
    {
        case 1:
            showStatusMessage(tr("Request update"));
            break;
        case 2:
             showStatusMessage(tr("Wait cmdline"));
            break;
        case 3:
             showStatusMessage(tr("Wait CCCC"));
            break;
        case 4:
            if(total == 0 && pos == 0)
            {
                ui->progressBar->setValue(100);
            }
            else
            {
                ui->progressBar->setValue(pos*100/total);
            }
            break;
        case 5:
            ui->btnUpdate->setEnabled(true);
            showInfoMessage(tr("Update successful"));
            break;
    }
}

void UploadWindow::debugMessage(int level, QString msg)
{
    ui->statusbar->showMessage(msg);
    qDebug() << msg;
}

bool UploadWindow::checkFile(QString name)
{
    QFile file(name);

    if(file.exists()){
        m_filename = name;
        return true;
    }

    return false;
}
void UploadWindow::on_pushButton_clicked()
{
    if(!checkFile(ui->lineEdit->text()))
    {
        QMessageBox::information(this,tr("msg_err_title"),tr("please select correct file"));
        return;
    }
    if(!checkChinese(ui->lineEdit->text()))
    {
        QString title = "请不要把升级文件放到中文目录";
        QMessageBox::information(this,tr("msg_err_title"),title);
        return;
    }
    ui->progressBar->setValue(0);
    ui->stackedWidget->setCurrentIndex(3);
   //QRect rc =  this->geometry();
   //rc.setHeight(300);
    //this->setGeometry(rc);
}



void UploadWindow::timerEvent(QTimerEvent *)
{
    listPorts();
}

void UploadWindow::on_stackedWidget_currentChanged(int arg1)
{
    if(arg1 == 0){
        listPorts();
    }
}
bool UploadWindow::checkChinese(QString file)
{
    for(int i = 0; i < file.length();i++)
    {
        QChar x = file[i];
        qDebug() <<x;
        if(x >= 0x7F)
        {
            return false;
        }
    }
    return true;
}
void UploadWindow::on_btnUpdate_clicked()
{
    if(!checkFile(ui->lineEdit->text()))
    {
        QMessageBox::information(this,tr("msg_err_title"),tr("please select correct file"));
        return;
    }


    wz.startUpdate(m_filename);
    showInfoMessage(tr("Please reset device and click ok button"));
}

void UploadWindow::on_pushButton_4_clicked()
{
    reloadLang("zh");
}
void UploadWindow::reloadLang(QString lang)
{
    QTranslator translator;
    if(lang == "zh")
    {
        translator.load(":/cn.qm");
    }
    else
    {
        translator.load(":/en.qm");
    }
    QCoreApplication::installTranslator(&translator);
    ui->stackedWidget->setCurrentIndex(1);
    ui->retranslateUi(this);
}

void UploadWindow::on_pushButton_2_clicked()
{
    reloadLang("en");
}
