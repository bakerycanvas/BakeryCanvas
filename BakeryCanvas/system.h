#pragma once

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

namespace BKSystem {
    enum MessageLevel { NORMAL, INFO, WARN, ERROR };
    bool showMessage(const char* title, const char* content, MessageLevel level);
}  // namespace BKSystem