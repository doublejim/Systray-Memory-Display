#ifndef TRAYICON
#define TRAYICON

#include <QCoreApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QPainter>
#include <QGraphicsScene>
#include <QTimer>
#include <QDebug>
#include "mainwindow.h"
#include "memoryusage.h"

class TrayIcon : public QObject
{
    Q_OBJECT

private:
    QMenu trayIconMenu;
    QSystemTrayIcon trayIcon;
    QAction* showSettingsAction;
    QAction* quitProgramAction;
    QMap <int, QIcon> allIcons;
    MemoryUsage memUsage;
    QTimer refreshTimer;
    SettingsWindow* window = 0;
    Settings* settings = 0;
    bool SHUTTING_DOWN = false;

    QColor COLOR_TEXT1;
    QColor COLOR_TEXT2;
    QColor COLOR_BACK1;
    QColor COLOR_BACK2;
    int REFRESH_INTERVAL;
    bool SHOW_VALUE;
    bool SHOW_BACKGROUND;
    bool SHOW_BACKGROUND_PROGRESS;

public:
    TrayIcon()
    {
        settings = new Settings("settings.json");
        //window = new SettingsWindow(settings);

        // Load settings, and prepare for change.
        loadSettings();
        connect(settings, &Settings::SigSettingsChanged, this, &TrayIcon::loadAndApplySettings);

        // Build context menu.
        showSettingsAction = new QAction("Settings...", this);
        connect(showSettingsAction, &QAction::triggered, this, &TrayIcon::showSettingsWindow);
        quitProgramAction = new QAction("Quit", this);
        connect(quitProgramAction, &QAction::triggered, this, &TrayIcon::quitProgram);

        trayIconMenu.addAction( showSettingsAction);
        trayIconMenu.addAction( quitProgramAction);

        trayIcon.setContextMenu(&trayIconMenu);

        // Allow tray icon to receive mouse inputs.
        connect(&trayIcon, &QSystemTrayIcon::activated, this, &TrayIcon::iconActivated);

        generateIconsWithCurrentSettings();

        // Start the refresh timer.
        refreshTimer.setInterval(REFRESH_INTERVAL);
        connect(&refreshTimer, &QTimer::timeout, this, &TrayIcon::refresh);
        refreshTimer.start();

        // Choose the icon to show, and display it!
        refresh();
        trayIcon.show();
    }

    ~TrayIcon()
    {
        SHUTTING_DOWN = true;
        delete settings; // meaning: save settings via its destructor.
    }

    void loadSettings()
    {
        REFRESH_INTERVAL = settings->value("refreshInterval").toInt();
        SHOW_VALUE = settings->value("showValue").toBool();
        SHOW_BACKGROUND = settings->value("showBackground").toBool();
        SHOW_BACKGROUND_PROGRESS = settings->value("showBackgroundProgress").toBool();
        COLOR_TEXT1.setNamedColor( settings->value("colorText1").toString());
        COLOR_TEXT2.setNamedColor( settings->value("colorText2").toString());
        COLOR_BACK1.setNamedColor( settings->value("colorBack1").toString());
        COLOR_BACK2.setNamedColor( settings->value("colorBack2").toString());
    }

    void generateIconsWithCurrentSettings()
    {
        for (int i = 0; i < 101; ++i)
            allIcons.insert(i, generateIcon(i));
    }

    void loadAndApplySettings()
    {
        refreshTimer.stop();

        loadSettings();
        refreshTimer.setInterval(REFRESH_INTERVAL);
        generateIconsWithCurrentSettings();
        refresh();

        refreshTimer.start();
    }

    void refresh()
    {
        if (SHUTTING_DOWN) return;
        int memoryUsed = memUsage.getMemoryUsed_inPercent();
        trayIcon.setIcon( allIcons.value( memoryUsed, generateIcon(0)));
        if (window != nullptr)
            window->setWindowIcon( allIcons.value( memoryUsed, generateIcon(0)));
        trayIcon.setToolTip("Memory Usage: " + QString::number(memoryUsed) + "%");
    }

    void showSettingsWindow()
    {
        if (window == nullptr)
        {
            window = new SettingsWindow(settings);
            window->exec();
            window->deleteLater();
            window = 0;
        }
    }

    void quitProgram()
    {
        qApp->exit(0);
    }

    void iconActivated(QSystemTrayIcon::ActivationReason reason)
    {
        switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::Context:
        {  // Both left and right click triggers this.
            QPoint pos = QCursor::pos();
            pos.setY( pos.y() - trayIcon.contextMenu()->height());
            trayIcon.contextMenu()->exec( pos);
            break;
        }
        // The rest do nothing.
        case QSystemTrayIcon::DoubleClick:
        case QSystemTrayIcon::MiddleClick:
        case QSystemTrayIcon::Unknown:
            break;
        }
    }

    QIcon generateIcon(int value)
    {
        QIcon icon;
        QSize size (48, 48);
        QImage image = QImage(size, QImage::Format_ARGB32);

        double usedAmount = value / 100.0;
        double freeAmount = 1 - usedAmount;

        QPainter painter( &image);
        QRect area1 (0, 0, image.width(), image.height() * freeAmount);
        QRect area2 (0, image.height() * freeAmount, image.width(), image.height());

        if (SHOW_BACKGROUND_PROGRESS)
        {
            painter.fillRect( area1, COLOR_BACK1);
            painter.fillRect( area2, COLOR_BACK2);
        } else if (SHOW_BACKGROUND) {
            painter.fillRect( image.rect(), COLOR_BACK1);
        }

        if (SHOW_VALUE)
        {
            painter.setPen( COLOR_TEXT1 );
            painter.setFont( QFont("Arial", 28, QFont::Bold));

            painter.setClipRect(area1);
            painter.drawText( image.rect(), Qt::AlignCenter, QString::number(value));

            if (SHOW_BACKGROUND_PROGRESS) painter.setPen( COLOR_TEXT2 );
            painter.setClipRect(area2);
            painter.drawText( image.rect(), Qt::AlignCenter, QString::number(value));
        }
        icon.addPixmap( QPixmap::fromImage( image));
        return icon;
    }
};

#endif // TRAYICON

