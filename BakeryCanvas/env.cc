#include "env.h"

namespace BKEnvironment {
    static bool initialized = false;
    static std::string _cwd = "";

    void init(const std::string& cwd) {
        char* buffer;
        if ((buffer = getcwd(nullptr, 0)) == nullptr) {
            perror("cannot get cwd");
            return;
        }
        std::string ret(buffer);
        free(buffer);
        _cwd = ret + "/" + cwd;
    }
    const std::string& cwd() {
        return _cwd;
    }
    std::string assets(const std::string& relativeFilename) {
        return _cwd + "/assets/" + relativeFilename;
    }
    bool exists(const std::string& filename) {
        if (FILE* file = fopen(filename.c_str(), "r")) {
            fclose(file);
            return true;
        }
        return false;
    }
}  // namespace BKEnvironment
