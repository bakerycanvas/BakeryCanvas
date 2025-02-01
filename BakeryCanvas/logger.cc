#include "logger.h"

namespace Logger {
    std::shared_ptr<spdlog::logger> stdoutLogger = nullptr;
    std::shared_ptr<spdlog::logger> stderrLogger = nullptr;
    std::shared_ptr<spdlog::logger> console = nullptr;
    void init() {

#ifdef DEBUG
        spdlog::set_level(spdlog::level::debug);
#endif

        stdoutLogger = spdlog::stdout_color_mt("stdout");
        auto stdoutLoggerSink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt *>(stdoutLogger->sinks().back().get());
        stdoutLoggerSink->set_color(spdlog::level::info, stdoutLoggerSink->white);
        stdoutLoggerSink->set_color(spdlog::level::debug, stdoutLoggerSink->blue);
        stdoutLoggerSink->set_color(spdlog::level::warn, stdoutLoggerSink->yellow);
        stdoutLoggerSink->set_color(spdlog::level::err, stdoutLoggerSink->red);
        stdoutLoggerSink->set_color_mode(spdlog::color_mode::always);

        stderrLogger = spdlog::stderr_color_mt("stderr");
        auto stderrLoggerSink = dynamic_cast<spdlog::sinks::stderr_color_sink_mt *>(stderrLogger->sinks().back().get());
        stderrLoggerSink->set_color(spdlog::level::info, stderrLoggerSink->red);
        stderrLoggerSink->set_color(spdlog::level::debug, stderrLoggerSink->red);
        stderrLoggerSink->set_color(spdlog::level::warn, stderrLoggerSink->red);
        stderrLoggerSink->set_color(spdlog::level::err, stderrLoggerSink->red);
        stderrLoggerSink->set_color_mode(spdlog::color_mode::always);

        console = spdlog::stdout_color_mt("console");
        auto consoleSink = dynamic_cast<spdlog::sinks::stdout_color_sink_mt *>(console->sinks().back().get());
        consoleSink->set_color(spdlog::level::info, consoleSink->magenta);
        consoleSink->set_color(spdlog::level::debug, consoleSink->magenta);
        consoleSink->set_color(spdlog::level::warn, consoleSink->yellow);
        consoleSink->set_color(spdlog::level::err, consoleSink->red);
        consoleSink->set_color_mode(spdlog::color_mode::always);
    }

};  // namespace Logger
