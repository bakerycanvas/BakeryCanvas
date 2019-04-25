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
		unsigned int levelFlag;
		if (level == MessageLevel::NORMAL) {
			levelFlag = MB_OK;
		}
		else if (level == MessageLevel::INFO) {
			levelFlag = MB_OK | MB_ICONINFORMATION;
		}
		else if (level == MessageLevel::WARN) {
			levelFlag = MB_OK | MB_ICONWARNING;
		}
		else if (level == MessageLevel::ERROR) {
			levelFlag = MB_OK | MB_ICONERROR;
		}
		//utf8 -> unicode16
		int wlen1 = MultiByteToWideChar(CP_UTF8, 0, title, -1, NULL, 0);
		wchar_t *wtitle = (wchar_t*)malloc(sizeof(wchar_t) * (wlen1 + 1));
		MultiByteToWideChar(CP_UTF8, 0, title, -1, wtitle, wlen1);
		wtitle[wlen1] = 0;
		int wlen2 = MultiByteToWideChar(CP_UTF8, 0, content, -1, NULL, 0);
		wchar_t *wcontent = (wchar_t*)malloc(sizeof(wchar_t) * (wlen2 + 1));
		MultiByteToWideChar(CP_UTF8, 0, content, -1, wcontent, wlen2);
		wcontent[wlen2] = 0;
		MessageBox(NULL, wcontent, wtitle, levelFlag);
		free(wtitle);
		free(wcontent);
#endif
        return false;
    }
}  // namespace BKSystem