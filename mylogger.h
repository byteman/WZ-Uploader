#ifndef MYLOGGER_H
#define MYLOGGER_H

#include <QObject>
class MyLogger
{
public:
    MyLogger();
    static bool init(QString iniFile,QString logFile);
};

#endif // MYLOGGER_H
