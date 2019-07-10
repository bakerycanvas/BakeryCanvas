#ifndef BAKERYCANVAS_ENV_H
#define BAKERYCANVAS_ENV_H

#include <string>
#include <unistd.h>

namespace BKEnvironment {
    void init(const std::string& cwd);
    const std::string& cwd();
    std::string assets(const std::string& relativeFilename);
    bool exists(const std::string& filename);
}  // namespace BKEnvironment

#endif  // BAKERYCANVAS_ENV_H
