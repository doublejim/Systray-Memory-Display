#ifndef SETTINGS
#define SETTINGS

#include "settingsdefaults.h"
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

class Settings : public QObject
{
    Q_OBJECT

signals:
    void SigSettingsChanged();

private:
    QJsonObject OBJECT;
    QJsonObject OBJECT_CHANGES;
    QString SAVE_FILE;

    void saveJson()
    {
        QFile qfile(SAVE_FILE);
        qfile.open( QIODevice::WriteOnly | QIODevice::Text);
        QJsonDocument saveDoc( OBJECT);
        qfile.write(saveDoc.toJson());
        qfile.close();
    }

    void openJson()
    {
        QFile qfile(SAVE_FILE);

        if (qfile.exists())
        {
            qfile.open( QIODevice::ReadOnly | QIODevice::Text);
            QByteArray jsonData = qfile.readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson( jsonData);
            OBJECT = jsonDoc.object();
            qfile.close();
        } else {
            OBJECT = DefaultSettings::object();
        }
    }

public:
    Settings(QString fileName)
    {
        SAVE_FILE = fileName;
        openJson();
    }

    ~Settings()
    {
        applyChangesAndSave();
    }

    QJsonValue value(const QString& key)
    {
        return OBJECT.value(key);
    }

    void insert(const QString& key, const QJsonValue &value)
    {
        OBJECT_CHANGES.insert(key, value);
    }

    void undoChanges()
    {
        OBJECT_CHANGES = QJsonObject();
    }

    void applyChanges()
    {
        QJsonObject::const_iterator it = OBJECT_CHANGES.begin();

        while (it != OBJECT_CHANGES.end())
        {
            OBJECT.insert( it.key(), it.value());
            ++it;
        }
        emit SigSettingsChanged();
    }

    void applyChangesAndSave()
    {
        applyChanges();
        saveJson();
    }

};

#endif // SETTINGS

