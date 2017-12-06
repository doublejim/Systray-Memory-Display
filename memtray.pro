#-------------------------------------------------
#
# Project created by QtCreator 2016-08-26T12:22:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = memtray
TEMPLATE = app

SOURCES += main.cpp \
    aboutdialog.cpp \
    trayicon.cpp \
    mainwindow.cpp \
    settings.cpp \
    memoryusage.cpp

HEADERS  += \
    aboutdialog.h \
    memoryusage.h \
    settings.h \
    settingsdefaults.h \
    trayicon.h \
    mainwindow.h

FORMS    += \
    aboutdialog.ui \
    mainwindow.ui

RESOURCES += \
    resources.qrc

RC_FILE = appicon.rc
