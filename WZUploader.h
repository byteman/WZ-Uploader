#ifndef UPDATEHANDLER_H
#define UPDATEHANDLER_H
#include <QThread>
#include <QString>

//class UpdateHandler : public CmdHandler,public QThread
 class WzUploader : public QThread
{
     Q_OBJECT
public:
    WzUploader();

    bool closePort();

    bool startUpdate(QString file);
    bool stopUpdate();
    void sendMessage(int result, int pos, int total);
    bool openPort(int port, int baud);
 signals:
    void updateResult(int result, int pos, int total);
    void debugMessage(QString msg);
private:
    QString readAll();
    void run();
    int m_port;
    int m_state;

    QString m_file;
    void writeString(QString msg);
    bool sendRequest();
    bool dorun();
 };

#endif // UPDATEHANDLER_H
