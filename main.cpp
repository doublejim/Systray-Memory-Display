#include "trayicon.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setQuitOnLastWindowClosed(false);

    TrayIcon* trayIcon = new TrayIcon;

    int result = a.exec();

    delete trayIcon;
    return result;
}
