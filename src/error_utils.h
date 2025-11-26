#pragma once

#include <QtLogging>

[[noreturn]] inline void unreachable(const char* msg)
{
    qFatal("%s", msg);
}
