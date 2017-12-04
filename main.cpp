#include "mainwindow.h"
#include "trayicon.h"
#include "settings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qApp->setQuitOnLastWindowClosed(false);
    TrayIcon ticon;

    return a.exec();
}
