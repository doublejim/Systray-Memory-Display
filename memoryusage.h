#ifndef MEMORYUSAGE
#define MEMORYUSAGE

#include <QObject>

// =========== WINDOWS ===========

#if defined(Q_OS_WIN)

#include <windows.h>

struct MemoryUsage
{
    MEMORYSTATUSEX m_statex;
    MemoryUsage();
    double getMemoryUsed_inPercent();
};
#endif

// =========== LINUX ===========

#if defined(Q_OS_LINUX)

#include <QFile>
#include <QRegularExpression>
#define BUFFER_SIZE 512

struct MemoryUsage
{
    bool m_availableMemory = true;
    QFile procMemInfoFile;
    char* bytes = nullptr;

    ~MemoryUsage();
    MemoryUsage();
    void setAvailableMemory( bool availableMemory);
    long long getValueFromBuffer( int& found, QString& readLine);
    double getMemoryUsed_inPercent();
};
#endif

#endif // MEMORYUSAGE

