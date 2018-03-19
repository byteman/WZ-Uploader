#include "WZUploader.h"
#include "Windows.h"
#include "pcomm/PCOMM.H"
#include <qdebug.h>

static bool bRecv = false;
static int postion = 0;
static int total   = 0;
static WzUploader *self = NULL;


//class Base_Thread:public QThread{
//Q_OBJECT
//public:
//    Base_Thread(int port):
//        m_port(port)
//    {

//    }
//    virtual bool doRun()=0;

//    void stopRun()
//    {
//        m_quit = true;
//        this->wait();
//    }
//    // QThread interface
//protected:
//    virtual void run()
//    {
//        while(!m_quit){
//            doRun();
//        }
//    }
//    int  m_port;
//    bool m_quit;
//};

//class Rx_Thread:public Base_Thread{
//    Q_OBJECT
//public:
//    Rx_Thread(int port):
//        Base_Thread(port)
//    {

//    }
//    QString readLine()
//    {
//        char buffer[128] = {0,};

//        sio_linput(m_port,buffer,128,'\n');

//        return buffer;

//    }
//    // QThread interface
//protected:
//    virtual bool doRun()
//    {
//        readLine();
//    }

//};

//class Tx_Thread:public Base_Thread{
//    Q_OBJECT
//public:
//    Tx_Thread(int port):
//        Base_Thread(port)
//    {
//        m_state  = 0;
//    }
//public slots:
//    void stateChange(int state)
//    {
//        m_state = state;
//    }
//    // QThread interface
//protected:
//    virtual bool doRun()
//    {
//        if(m_state > 0)
//        {
//            switch(m_state)
//            {
//                case 1:
//                    sendRequest();
//                    break;
//                case 2:
//                case 3:
//                    sendMessage(m_state,0,0);
//            }
//            if(bRecv)
//            {
//                //读取全部收到的数据.

//                QString rxStr = readAll();
//                qDebug() << "recv:" << rxStr;

//                if(m_state == 1)
//                {
//                    writeString("\r");
//                    //sio_write(m_port,"\r",1);
//                    m_state = 2;
//                }
//    #if 1
//                else if(m_state == 2)
//                {
//                    if(rxStr.contains("MS:"))
//                    {
//                        qDebug() << "send update 1";
//                        sleep(3);
//                        qDebug() << "send update 2";
//                        writeString("update\r\n");

//                        m_state = 3;
//                    }
//                }
//                else if(m_state == 3)
//                {
//                    qDebug() << "ready send update";
//                    if(rxStr.contains("C"))
//                    //sleep(7);
//                    {
//                        qDebug() << tr("ready send file") << m_file;

//                        int err = sio_FtYmodemTx(m_port,(char*)m_file.toStdString().c_str(),xCallback,27);
//                        if(0 != err)
//                        {
//                            qDebug() << tr("start failed") << err;
//                            sendMessage(-1,0,0);
//                            m_state  = 0;

//                        }
//                        else
//                        {
//                            m_state = 4;

//                        }

//                    }
//                }
//                else if(m_state == 4)
//                {
//                    if(rxStr.contains("Programming Completed Successfully"))
//                    {
//                        sendMessage(5,0,0);
//                        qDebug() << "update ok";
//                        writeString("reset\r\n");
//                        m_state = 0;
//                    }
//                }
//    #endif
//                bRecv = false;

//            }
//            return true;
//        }
//        return false;
//    }
//private:
//    int m_state;

//};
WzUploader::WzUploader():
    m_state(0),
    m_port(0),
    m_sleep_ms(100)
{
    self = this;
}

bool WzUploader::closePort()
{
    if(m_port <= 0) return true;
    return (sio_close(m_port)==SIO_OK);

}
VOID CALLBACK CntIrq(int port)
{
    /* You should NOT read data at this point. */
    /* Send message to let main program to read data. */
    bRecv = true;
    qDebug() <<"cntIrq";
}
bool WzUploader::openPort(int port,int baud)
{
    int err = sio_open(port);
    if(err != SIO_OK)
    {
        qDebug() << "openPort err=" << err;
        return false;
    }
    err  = sio_ioctl(port,B9600,P_NONE | BIT_8 | STOP_1);
    if(err != SIO_OK)
    {
        return false;
    }
    //读取总超时时间100ms,每个字符超时间隔100ms
    sio_SetReadTimeouts(m_port,1000,100);
    sio_cnt_irq(m_port,CntIrq,1);

    m_port = port;
    return true;

}

void WzUploader::sendMessage(int result, int pos, int total)
{
    emit updateResult(result,pos,total);
}

void WzUploader::sendDbgMessage(int level, QString msg)
{
    emit debugMessage(level,msg);
}

QString WzUploader::readAll()
{
    char buffer[128] = {0,};

    sio_read(m_port,buffer,128);

    return buffer;

}
QString WzUploader::readLine()
{
    char buffer[128] = {0,};

    sio_linput(m_port,buffer,128,'\n');

    return buffer;

}
void WzUploader::writeString(QString msg)
{
    sio_write(m_port,msg.toLatin1().data(),msg.length());
}

int CALLBACK xCallback(long xmitlen, int buflen, char *buf, long flen)
{
/* Update receiving status */
    //qDebug() << "send -> " << xmitlen << " total --> " <<flen;
    postion = xmitlen;
    total = flen;
    self->sendMessage(4,postion,total);
    return 0;
}
bool WzUploader::sendRequest()
{
    //发送空格，等待回应.
    int err = sio_write(m_port,"                   ",25);
    if(err != 25)
    {
        QString msg = QString("sendReq failed err=%1").arg(err);
        sendDbgMessage(ERR_LEVEL,msg);
    }
    m_sleep_ms = 100;
    return true;
}
bool WzUploader::dorun()
{
    if(m_state > 0)
    {

        switch(m_state)
        {
            case 1:
                sendRequest();
                break;
            case 2:
            case 3:
            case 4:
                sendMessage(m_state,0,0);
                m_sleep_ms = 1000;
        }

            //读取全部收到的数据.

            QString rxStr = readAll();
            sendDbgMessage(TRACE_LEVEL, rxStr);

            if(rxStr.length() <= 0)
            {
                return true;
            }
            //如果是在请求升级状态，收到了数据，表示已经进入了升级状态.
            if(m_state == 1)
            {

                writeString("\r");
                m_state = 2;
            }
#if 1
            else if(m_state == 2)
            {
                if(rxStr.contains("MS:"))
                {
                    writeString("update\r\n");
                    m_sleep_ms = 1000;
                    m_state = 3;
                    m_count = 0;
                }
            }
            else if(m_state == 3)
            {
                sendDbgMessage(DEBUG_LEVEL,tr("Start update"));
                if(rxStr.contains("C"))
                //sleep(7);
                {
                    //qDebug() << tr("ready send file") << m_file;

                    int err = sio_FtYmodemTx(m_port,(char*)m_file.toStdString().c_str(),xCallback,27);
                    if( err < 0)
                    {
                        //qDebug() << tr("start failed") << err;
                        sendMessage(-1,0,0);
                        m_state  = 0;

                    }
                    else
                    {
                        //qDebug() << "send ok";
                        m_state = 4;

                    }

                }
            }
            else if(m_state == 4)
            {
                if(rxStr.contains("Successfully"))
                {
                    sendMessage(5,0,0);
                    //qDebug() << "update ok";
                    writeString("reset\r\n");
                    m_state = 0;
                }
                return false;
            }
#endif


        }

    return true;
}
void WzUploader::Flush()
{
    sio_flush(m_port,2);
}
bool WzUploader::startUpdate(QString file)
{

    m_file = file;
    Flush();
    m_state = 1;
    this->start();
    return true;
}

bool WzUploader::stopUpdate()
{
    m_state = 0;
    closePort();
    this->wait();
    return true;
}


void WzUploader::run()
{
    qDebug()<<"thread enter";
    while(dorun())
    {
        this->msleep(m_sleep_ms);
    }
    qDebug() << "thread exit";

}
