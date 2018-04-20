#include "mainwindow.h"
#include <QApplication>
#include "uploadwindow.h"
#include <QTranslator>
#include <QSettings>
#include "mylogger.h"
#include <QDebug>
void loadLang(QTranslator& translator)
{

    QSettings config("wface.ini", QSettings::IniFormat);

    QString lang       = config.value("/config/lang","en").toString();
    if(lang == "zh")
    {
        translator.load(":/cn.qm");
    }
    else
    {
        translator.load(":/en.qm");
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    MyLogger::init("config.ini","WzUploader.log");
    qDebug() << "start app";
    loadLang(translator);
    a.installTranslator(&translator);
    UploadWindow w;
    w.show();

    return a.exec();
}
