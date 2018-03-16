#-------------------------------------------------
#
# Project created by QtCreator 2018-03-15T14:57:03
#
#-------------------------------------------------

QT       += core gui
QT +=  serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WZ_Uploader
TEMPLATE = app
win32:LIBS += -L$$PWD/3rdparty/pcomm/ -lPCOMM
win32:LIBS += -lsetupapi -lwsock32 -lws2_32 -lAdvapi32
INCLUDEPATH+=./3rdparty
SOURCES += main.cpp\
        mainwindow.cpp \
    WZUploader.cpp

HEADERS  += mainwindow.h \
    WZUploader.h

FORMS    += mainwindow.ui
