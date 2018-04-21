#ifndef UPLOADWINDOW_H
#define UPLOADWINDOW_H

#include <QMainWindow>
#include "WZUploader.h"
namespace Ui {
class UploadWindow;
}

class UploadWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UploadWindow(QWidget *parent = 0);
    ~UploadWindow();

protected slots:
    void updateResult(int result, int pos, int total);
    void debugMessage(int level, QString msg);
private slots:

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_clicked();


    void on_stackedWidget_currentChanged(int arg1);

    void on_btnUpdate_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::UploadWindow *ui;
    QString m_filename;
    WzUploader wz;
    void listPorts();
    bool getPort(QString strPort, int &iPort);
    bool checkFile(QString name);
    void showErrMessage(QString msg);
    void showInfoMessage(QString msg);
    void showStatusMessage(QString msg);

    // QObject interface
    void reloadLang(QString lang);
    bool checkChinese(QString file);
protected:
    virtual void timerEvent(QTimerEvent *);
};

#endif // UPLOADWINDOW_H
