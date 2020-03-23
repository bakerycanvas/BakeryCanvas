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
        stderrLogger = spdlog::stderr_color_mt("stderr");
        console = spdlog::stdout_color_mt("console");
    }

};  // namespace Logger
