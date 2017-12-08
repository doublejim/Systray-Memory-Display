#include "settings.h"


void Settings::saveJson()
{
    QFile qfile(m_saveFile);
    qfile.open( QIODevice::WriteOnly | QIODevice::Text);
    QJsonDocument saveDoc( m_object);
    qfile.write(saveDoc.toJson());
    qfile.close();
}

void Settings::openJson()
{
    QFile qfile(m_saveFile);

    if (qfile.exists())
    {
        qfile.open( QIODevice::ReadOnly | QIODevice::Text);
        QByteArray jsonData = qfile.readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson( jsonData);
        m_object = jsonDoc.object();
        qfile.close();
    } else {
        m_object = DefaultSettings::object();
    }
}

Settings::Settings(QString fileName)
{
    m_saveFile = fileName;
    openJson();
}

Settings::~Settings()
{
    saveJson();
}

QJsonValue Settings::value(const QString &key)
{
    return m_object.value(key);
}

void Settings::insert(const QString &key, const QJsonValue &value)
{
    m_objectChanges.insert(key, value);
}

void Settings::undoChanges()
{
    m_objectChanges = QJsonObject();
}

void Settings::applyChanges()
{
    QJsonObject::const_iterator it = m_objectChanges.begin();

    while (it != m_objectChanges.end())
    {
        m_object.remove( it.key());
        m_object.insert( it.key(), it.value());
        ++it;
    }
    emit sSettingsChanged();
}

void Settings::applyChangesAndSave()
{
    applyChanges();
    saveJson();
}
