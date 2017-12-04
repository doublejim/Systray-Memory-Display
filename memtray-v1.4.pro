#-------------------------------------------------
#
# Project created by QtCreator 2016-08-26T12:22:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = memtray-v1.4
TEMPLATE = app


SOURCES += main.cpp\
    settingswindow.cpp \
    aboutdialog.cpp

HEADERS  += \
    trayicon.h \
    memoryusage.h \
    settings.h \
    settingsdefaults.h \
    settingswindow.h \
    aboutdialog.h

FORMS    += \
    settingswindow.ui \
    aboutdialog.ui

RESOURCES += \
    resources.qrc

RC_FILE = appicon.rc
