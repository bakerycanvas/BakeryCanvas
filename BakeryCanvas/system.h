#pragma once

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#ifdef WIN32
#include <Windows.h>
#undef ERROR
#endif

namespace BKSystem {
    enum MessageLevel { NORMAL, INFO, WARN, ERROR };
    bool showMessage(const char* title, const char* content, MessageLevel level);
}  // namespace BKSystem