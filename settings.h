#ifndef SETTINGS
#define SETTINGS

#include "defaultsettings.h"
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

class Settings : public QObject
{
    Q_OBJECT

signals:
    void sSettingsChanged();

private:
    QJsonObject m_object;
    QJsonObject m_objectChanges;
    QString m_saveFile;
    void saveJson();
    void openJson();

public:
    Settings(QString fileName);
    ~Settings();
    QJsonValue value(const QString& key);
    void insert(const QString& key, const QJsonValue &value);
    void undoChanges();
    void applyChanges();
    void applyChangesAndSave();
};

#endif // SETTINGS

