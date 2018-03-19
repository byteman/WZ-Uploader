#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "WZUploader.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void updateResult(int result, int pos, int total);
    void debugMessage(int level,QString msg);
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    void listPorts();
    WzUploader wz;
    bool getPort(QString strPort, int &iPort);

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *);
};


#endif // MAINWINDOW_H
