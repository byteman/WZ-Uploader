#ifndef UPDATEHANDLER_H
#define UPDATEHANDLER_H
#include <QThread>
#include <QString>
enum MSG_LEVEL{
    TRACE_LEVEL=0,
    DEBUG_LEVEL,
    ERR_LEVEL
};
//class UpdateHandler : public CmdHandler,public QThread
 class WzUploader:public QThread
{
     Q_OBJECT
public:
    WzUploader();

    bool closePort();

    bool startUpdate(QString file);
    bool stopUpdate();
    void sendMessage(int result, int pos, int total);
    void sendDbgMessage(int level,QString msg);
    bool openPort(int port, int baud);
 signals:
    void updateResult(int result, int pos, int total);
    void debugMessage(int level,QString msg);
private:
    QString readAll();
    void run();
    int m_port;
    int m_state;
    int m_count;
    int m_sleep_ms;
    QThread m_rx_thread;
    QThread m_tx_thread;

    QString m_file;
    void writeString(QString msg);
    bool sendRequest();
    bool dorun();
    QString readLine();
    void Flush();
 };

#endif // UPDATEHANDLER_H
