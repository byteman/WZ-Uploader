#include "mylogger.h"
#include <QSettings>
#include "ConsoleAppender.h"
#include "FileAppender.h"
MyLogger::MyLogger()
{

}
static void initLogger(QString iniFile,QString logFile)
{

    QSettings config(iniFile, QSettings::IniFormat);

    bool enable_log        = config.value("/config/log",false).toBool();

    if(!enable_log)
    {
        return;
    }
    ConsoleAppender* consoleAppender = new ConsoleAppender;
    consoleAppender->setFormat("[%{type:-7}] <%{Function}> %{message}\n");
    cuteLogger->registerAppender(consoleAppender);

    cuteLogger->registerAppender(new FileAppender(logFile));

}
bool MyLogger::init(QString iniFile,QString logFile)
{
    initLogger(iniFile,logFile);
    return true;
}

