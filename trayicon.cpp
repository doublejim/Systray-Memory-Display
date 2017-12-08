#include "trayicon.h"

TrayIcon::TrayIcon():
    settings (new Settings("settings.json"))
{
    // Load settings, and prepare for settings change.
    loadSettings();
    connect(settings, &Settings::sSettingsChanged, this, &TrayIcon::loadAndApplySettings);

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

    // Connect the refresh timer.
    connect(&refreshTimer, &QTimer::timeout, this, &TrayIcon::refresh);

    // Apply settings and start refreshing.
    applySettings();
    refresh();
    refreshTimer.start();

    // Display the tray icon!
    trayIcon.show();
}

TrayIcon::~TrayIcon()
{
    m_shuttingDown = true;

    delete showSettingsAction;
    delete quitProgramAction;
    delete settings;            // Meaning: save settings via its destructor. Then free memory.
}

void TrayIcon::loadSettings()
{
    m_refreshInterval = settings->value("refreshInterval").toInt();
    m_showValue = settings->value("showValue").toBool();
    m_showBackground = settings->value("showBackground").toBool();
    m_showBackgroundProgress = settings->value("showBackgroundProgress").toBool();
    m_colorText1.setNamedColor( settings->value("colorText1").toString());
    m_colorText2.setNamedColor( settings->value("colorText2").toString());
    m_colorBack1.setNamedColor( settings->value("colorBack1").toString());
    m_colorBack2.setNamedColor( settings->value("colorBack2").toString());
    m_availableMemory = settings->value("availableMemory").toBool();
}

void TrayIcon::generateIconsWithCurrentSettings()
{
    allIcons.clear();
    for (int i = 0; i <= 100; ++i)
        allIcons.insert(i, generateIcon(i));
}

void TrayIcon::applySettings()
{
    refreshTimer.setInterval(m_refreshInterval);

    // Set option on memUsage object.
    #if defined(Q_OS_LINUX)
        memUsage.setAvailableMemory( m_availableMemory);
    #endif

    generateIconsWithCurrentSettings();
}

void TrayIcon::loadAndApplySettings()
{
    refreshTimer.stop();
    loadSettings();
    applySettings();
    refresh();
    refreshTimer.start();
}

void TrayIcon::refresh()
{
    if (m_shuttingDown)
        return;

    int memoryUsed = memUsage.getMemoryUsed_inPercent();
    trayIcon.setIcon( allIcons.value( memoryUsed, generateIcon(0)));

    if (window != nullptr)
        window->setWindowIcon( allIcons.value( memoryUsed, generateIcon(0)));

    trayIcon.setToolTip("Memory Usage: " + QString::number(memoryUsed) + "%");
}

void TrayIcon::showSettingsWindow()
{
    if (window != nullptr)
        return;

    window = new MainWindow( settings);
    window->setAttribute( Qt::WA_DeleteOnClose);
    window->show();
    window = nullptr;
}

void TrayIcon::quitProgram()
{
    qApp->quit();           // quit the event loop. then run trayicon destructor in main.
}

void TrayIcon::iconActivated(QSystemTrayIcon::ActivationReason reason)
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

QIcon TrayIcon::generateIcon(int value)
{
    QIcon icon;
    QSize size (48, 48);
    QImage image = QImage(size, QImage::Format_ARGB32);

    double usedAmount = value / 100.0;
    double freeAmount = 1 - usedAmount;

    QPainter painter( &image);
    QRect area1 (0, 0, image.width(), image.height() * freeAmount);
    QRect area2 (0, image.height() * freeAmount, image.width(), image.height());

    if (m_showBackgroundProgress)
    {
        painter.fillRect( area1, m_colorBack1);
        painter.fillRect( area2, m_colorBack2);
    } else if (m_showBackground) {
        painter.fillRect( image.rect(), m_colorBack1);
    }

    if (m_showValue)
    {
        painter.setPen( m_colorText1 );
        painter.setFont( QFont("Arial", 28, QFont::Bold));

        painter.setClipRect(area1);
        painter.drawText( image.rect(), Qt::AlignCenter, QString::number(value));

        if (m_showBackgroundProgress) painter.setPen( m_colorText2 );
        painter.setClipRect(area2);
        painter.drawText( image.rect(), Qt::AlignCenter, QString::number(value));
    }
    icon.addPixmap( QPixmap::fromImage( image));
    return icon;
}
