#ifndef DEFAULTSETTINGS_H
#define DEFAULTSETTINGS_H

#include <QJsonObject>

struct DefaultSettings
{
    static QJsonObject object()
    {
        QJsonObject defaultSettings;
        defaultSettings.insert("colorBack1", "#140f0f");
        defaultSettings.insert("colorBack2", "#3d1c4b");
        defaultSettings.insert("colorText1", "#ffffff");
        defaultSettings.insert("colorText2", "#ffffff");
        defaultSettings.insert("refreshInterval", 1000);
        defaultSettings.insert("showBackground", false);
        defaultSettings.insert("showBackgroundProgress", true);
        defaultSettings.insert("showValue", true);
        defaultSettings.insert("availableMemory", true);
        return defaultSettings;
    }
};

#endif // DEFAULTSETTINGS_H

