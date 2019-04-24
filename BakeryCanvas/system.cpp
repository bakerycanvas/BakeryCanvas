#include "system.h"

namespace BKSystem {

    bool showMessage(const char* title, const char* content, MessageLevel level) {
#ifdef __APPLE__
        CFStringRef headerText = CFStringCreateWithCString(NULL, title, kCFStringEncodingUTF8);
        CFStringRef messageText = CFStringCreateWithCString(NULL, content, kCFStringEncodingUTF8);
        CFOptionFlags uiresult;

        int levelFlag;
        if (level == MessageLevel::NORMAL) {
            levelFlag = kCFUserNotificationPlainAlertLevel;
        } else if (level == MessageLevel::INFO) {
            levelFlag = kCFUserNotificationNoteAlertLevel;
        } else if (level == MessageLevel::WARN) {
            levelFlag = kCFUserNotificationCautionAlertLevel;
        } else if (level == MessageLevel::ERROR) {
            levelFlag = kCFUserNotificationStopAlertLevel;
        }

        CFUserNotificationDisplayAlert(0, levelFlag, NULL, NULL, NULL, headerText, messageText, NULL, NULL, NULL, &uiresult);

        CFRelease(headerText);
        CFRelease(messageText);

        return uiresult == kCFUserNotificationDefaultResponse;
#endif

#ifdef WIN32
#endif
        return false;
    }
}  // namespace BKSystem