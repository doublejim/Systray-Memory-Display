#ifndef MEMORYUSAGE
#define MEMORYUSAGE

#include <QObject>

// =========== WINDOWS ===========

#ifdef _WIN32
#include <windows.h>

struct MemoryUsage
{
    MEMORYSTATUSEX statex;
    MemoryUsage();
    double getMemoryUsed_inPercent();
}
#endif

// =========== LINUX ===========

#ifdef __linux__

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

