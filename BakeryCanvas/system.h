#pragma once

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#include <string>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#ifdef WIN32
#include <Windows.h>
#undef ERROR
#include <direct.h>
#define GetCurrentDir _getcwd
#endif

namespace BKSystem {
    enum MessageLevel { NORMAL, INFO, WARN, ERROR };
    bool showMessage(const char* title, const char* content, MessageLevel level);
    std::string getCWD();

}  // namespace BKSystem