#-------------------------------------------------
#
# Project created by QtCreator 2016-08-26T12:22:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11

TARGET = memtray
TEMPLATE = app

SOURCES += main.cpp \
    aboutdialog.cpp \
    trayicon.cpp \
    settings.cpp \
    memoryusage.cpp \
    mainwindow.cpp

HEADERS  += \
    aboutdialog.h \
    memoryusage.h \
    settings.h \
    trayicon.h \
    mainwindow.h \
    defaultsettings.h

FORMS    += \
    aboutdialog.ui \
    mainwindow.ui

RESOURCES += \
    resources.qrc

RC_FILE = appicon.rc
