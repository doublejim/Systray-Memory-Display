#include "memoryusage.h"

// =========== LINUX ===========

#if defined(Q_OS_WIN)

MemoryUsage::MemoryUsage()
{
    m_statex.dwLength = sizeof (m_statex);
}

double MemoryUsage::getMemoryUsed_inPercent()
{
    GlobalMemoryStatusEx (&m_statex);
    qint64 totalBytes = m_statex.ullTotalPhys;
    qint64 freeBytes = m_statex.ullAvailPhys;
    return 100 - 100.0 * freeBytes / totalBytes;
}
#endif

// =========== LINUX ===========

#if defined(Q_OS_LINUX)
MemoryUsage::~MemoryUsage() {
    delete[] bytes;
}

MemoryUsage::MemoryUsage()
{
    procMemInfoFile.setFileName("/proc/meminfo");
    bytes = new char[BUFFER_SIZE];
}

void MemoryUsage::setAvailableMemory(bool availableMemory)
{
    m_availableMemory = availableMemory;
}

long long MemoryUsage::getValueFromBuffer(int &found, QString &readLine)
{
    ++found;
    QRegularExpression re (" (\\d+) kB");
    QRegularExpressionMatch match = re.match( readLine);

    if (match.hasMatch())
        return match.captured(1).toLongLong();
    return -1;
}

double MemoryUsage::getMemoryUsed_inPercent()
{
    long long totalKb = 0;
    long long freeKb = 0;
    long long availableKb = 0;

    if (!procMemInfoFile.open( QIODevice::ReadOnly | QIODevice::Text))
        return -1;

    int found = 0;

    do {
        if (-1 == procMemInfoFile.readLine( bytes, BUFFER_SIZE))
            break;

        QString readLine = bytes;

        if (readLine.startsWith("MemTotal"))
            totalKb = getValueFromBuffer( found, readLine);
        else if (readLine.startsWith("MemFree"))
            freeKb = getValueFromBuffer( found, readLine);
        else if (readLine.startsWith("MemAvailable"))
            availableKb = getValueFromBuffer( found, readLine);
    } while (found < 3);

    procMemInfoFile.close();

    if (m_availableMemory)
        return 100 - 100.0 * availableKb / totalKb;
    else
        return 100 - 100.0 * freeKb / totalKb;
}
#endif
