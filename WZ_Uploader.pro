#-------------------------------------------------
#
# Project created by QtCreator 2018-03-15T14:57:03
#
#-------------------------------------------------

QT       += core gui
QT +=  serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WZUploader
TEMPLATE = app
win32:LIBS += -L$$PWD/3rdparty/pcomm/ -lCuteLogger -lPCOMM
win32:LIBS += -lsetupapi -lwsock32 -lws2_32 -lAdvapi32
INCLUDEPATH+=./3rdparty ./3rdparty/CuteLogger/include
SOURCES += main.cpp\
        mainwindow.cpp \
    WZUploader.cpp \
    uploadwindow.cpp \
    utils.cpp \
    mylogger.cpp

HEADERS  += mainwindow.h \
    WZUploader.h \
    uploadwindow.h \
    utils.h \
    mylogger.h

FORMS    += mainwindow.ui \
    uploadwindow.ui
TRANSLATIONS+=cn.ts en.ts

RESOURCES += \
    myrc.qrc
RC_FILE=icon.rc
DISTFILES += \
    cn.qm \
    en.qm
