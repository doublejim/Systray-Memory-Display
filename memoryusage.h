#ifndef MEMORYUSAGE
#define MEMORYUSAGE

#ifdef _WIN32
    #include <qt_windows.h>
#endif
#ifdef __linux__
    #include "sys/types.h"
    #include "sys/sysinfo.h"
#endif
#include <QObject>

struct MemoryUsage
{
    #ifdef _WIN32
        MEMORYSTATUSEX statex;
    #endif
    #ifdef __linux__
        struct sysinfo memInfo;
    #endif

    MemoryUsage()
    {
        #ifdef _WIN32
            statex.dwLength = sizeof (statex);
        #endif
    }

    double getMemoryUsed_inPercent()
    {
        #ifdef _WIN32
            GlobalMemoryStatusEx (&statex);
            qint64 totalBytes = statex.ullTotalPhys;
            qint64 freeBytes = statex.ullAvailPhys;
            return 100 - 100.0 * freeBytes / totalBytes;
        #endif
        #ifdef __linux__
            sysinfo (&memInfo);
            long long totalBytes = memInfo.totalram * memInfo.mem_unit;
            long long freeBytes = memInfo.freeram * memInfo.mem_unit;
            return 100 - 100.0 * freeBytes / totalBytes;
        #endif
    }
};

#endif // MEMORYUSAGE

