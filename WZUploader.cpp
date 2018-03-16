#include "WZUploader.h"
#include "Windows.h"
#include "pcomm/PCOMM.H"
#include <qdebug.h>

static bool bRecv = false;
static int postion = 0;
static int total   = 0;
static WzUploader *self = NULL;
WzUploader::WzUploader():
    m_state(0),
    m_port(0)
{
    self = this;
}

bool WzUploader::closePort()
{
    if(m_port <= 0) return true;
    return (sio_close(m_port)==SIO_OK);

}
static VOID CALLBACK CntIrq(int port)
{
    /* You should NOT read data at this point. */
    /* Send message to let main program to read data. */
    bRecv = true;
    qDebug() <<"cntIrq";
}
bool WzUploader::openPort(int port,int baud)
{
    if(sio_open(port) != SIO_OK)
    {

        return false;
    }
    if(sio_baud(m_port,baud) != SIO_OK)
    {
        return false;
    }
    //读取总超时时间100ms,每个字符超时间隔100ms
    sio_SetReadTimeouts(m_port,100,100);
    sio_cnt_irq(m_port,CntIrq,1);

    m_port = port;
    return true;

}

void WzUploader::sendMessage(int result, int pos, int total)
{
    emit updateResult(result,pos,total);
}

QString WzUploader::readAll()
{
    char buffer[128] = {0,};

    sio_read(m_port,buffer,128);

    return buffer;

}
void WzUploader::writeString(QString msg)
{
    sio_write(m_port,msg.toLatin1().data(),msg.length());
}

int CALLBACK xCallback(long xmitlen, int buflen, char *buf, long flen)
{
/* Update receiving status */
    qDebug() << "send -> " << xmitlen << " total --> " <<flen;
    postion = xmitlen;
    total = flen;
    self->sendMessage(4,postion,total);
    return 0;
}
bool WzUploader::sendRequest()
{
    //发送空格，等待回应.
    sio_write(m_port,"                   ",25);
    sendMessage(1,0,0);
    qDebug() << "send space";
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
                sendMessage(m_state,0,0);
        }
        if(bRecv)
        {
            //读取全部收到的数据.

            QString rxStr = readAll();
            qDebug() << "recv:" << rxStr;

            if(m_state == 1)
            {
//                if(rxStr.contains("return key.\r\n"))
//                {
//                    qDebug() << "get return key";
//                    sio_write(m_port,"\r",1);
//                    m_state = 2;
//                }
                writeString("\r");
                //sio_write(m_port,"\r",1);
                m_state = 2;
            }
#if 1
            else if(m_state == 2)
            {
                if(rxStr.contains("MS:"))
                {
                    qDebug() << "send update 1";
                    sleep(3);
                    qDebug() << "send update 2";
                    writeString("update\r\n");

                    m_state = 3;
                }
            }
            else if(m_state == 3)
            {
                qDebug() << "ready send update";
                if(rxStr.contains("C"))
                //sleep(7);
                {
                    qDebug() << tr("ready send file") << m_file;

                    int err = sio_FtYmodemTx(m_port,(char*)m_file.toStdString().c_str(),xCallback,27);
                    if(0 != err)
                    {
                        qDebug() << tr("start failed") << err;
                        sendMessage(-1,0,0);
                        m_state  = 0;

                    }
                    else
                    {
                        m_state = 4;

                    }

                }
            }
            else if(m_state == 4)
            {
                if(rxStr.contains("Programming Completed Successfully"))
                {
                    sendMessage(5,0,0);
                    qDebug() << "update ok";
                    writeString("reset\r\n");
                    m_state = 0;
                }
            }
#endif
            bRecv = false;

        }
        return true;
    }
    return false;
}

bool WzUploader::startUpdate(QString file)
{

    m_file = file;

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

    while(dorun())
    {
        this->msleep(100);
    }

}
