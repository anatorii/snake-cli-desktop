#ifndef APP_PATH_H
#define APP_PATH_H

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

class AppPath {
   private:
    std::string app_dir;

    static std::string getExecutablePath() {
        char buffer[PATH_MAX];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) {
            buffer[len] = '\0';
            return std::string(buffer);
        }
        return "";
    }

   public:
    AppPath() {
        std::string exe_path = getExecutablePath();
        if (!exe_path.empty()) {
            fs::path path(exe_path);
            app_dir = path.parent_path().string();
        } else {
            app_dir = fs::current_path().string();
        }
    }

    std::string getResourcePath(const std::string& relative_path) const {
        return (fs::path(app_dir) / relative_path).string();
    }

    bool loadFile(const std::string& filename, std::string& content) const {
        std::string full_path = getResourcePath(filename);
        std::ifstream file(full_path);
        if (!file.is_open()) {
            return false;
        }

        content = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return true;
    }
};

#endif
