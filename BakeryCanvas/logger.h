#ifndef BAKERYCANVAS_LOGGER_H
#define BAKERYCANVAS_LOGGER_H

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
//#include <spdlog/sinks/daily_file_sink.h.h>

namespace Logger {
    extern std::shared_ptr<spdlog::logger> stdoutLogger;
    extern std::shared_ptr<spdlog::logger> stderrLogger;
    extern std::shared_ptr<spdlog::logger> console;
    extern void init();

    template <typename... Args> void debug(spdlog::string_view_t fmt, const Args&... args) {
        stdoutLogger->debug(fmt, args...);
    }
    template <typename... Args> void info(spdlog::string_view_t fmt, const Args&... args) {
        stdoutLogger->info(fmt, args...);
    }
    template <typename... Args> void warn(spdlog::string_view_t fmt, const Args&... args) {
        stdoutLogger->warn(fmt, args...);
    }
    template <typename... Args> void error(spdlog::string_view_t fmt, const Args&... args) {
        stderrLogger->error(fmt, args...);
    }
    template <typename... Args> void fatal(spdlog::string_view_t fmt, const Args&... args) {
        stderrLogger->critical(fmt, args...);
    }

    namespace Console {
        template <typename... Args> void debug(spdlog::string_view_t fmt, const Args&... args) {
            console->debug(fmt, args...);
        }
        template <typename... Args> void info(spdlog::string_view_t fmt, const Args&... args) {
            console->info(fmt, args...);
        }
        template <typename... Args> void warn(spdlog::string_view_t fmt, const Args&... args) {
            console->warn(fmt, args...);
        }
        template <typename... Args> void error(spdlog::string_view_t fmt, const Args&... args) {
            console->error(fmt, args...);
        }
        template <typename... Args> void fatal(spdlog::string_view_t fmt, const Args&... args) {
            console->critical(fmt, args...);
        }
    }  // namespace Console

};  // namespace Logger

#endif  // BAKERYCANVAS_LOGGER_H
