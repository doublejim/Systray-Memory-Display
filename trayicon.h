#ifndef TRAYICON
#define TRAYICON

#include <QSystemTrayIcon>
#include <QMenu>
#include <QPainter>
#include <QGraphicsScene>
#include <QTimer>
#include <QScreen>
#include <QGuiApplication>
#include <QDebug>

#include "mainwindow.h"
#include "memoryusage.h"

class TrayIcon : public QObject
{
    Q_OBJECT
private:
    QMenu trayIconMenu;
    QSystemTrayIcon trayIcon;
    QMap <int, QIcon> allIcons;
    MemoryUsage memUsage;
    QTimer refreshTimer;
    bool m_shuttingDown = false;
    QAction* showSettingsAction = nullptr;
    QAction* quitProgramAction = nullptr;
    Settings* settings = nullptr;
    MainWindow* window = nullptr;

    QColor m_colorText1;
    QColor m_colorText2;
    QColor m_colorBack1;
    QColor m_colorBack2;
    int m_refreshInterval;
    bool m_showValue;
    bool m_showBackground;
    bool m_showBackgroundProgress;
    bool m_availableMemory;

    void loadSettings();
    void generateIconsWithCurrentSettings();
    void loadAndApplySettings();
    void refresh();
    void showSettingsWindow();
    void quitProgram();
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    QIcon generateIcon(int value);
    void applySettings();

public:
    TrayIcon();
    ~TrayIcon();
};

#endif // TRAYICON

