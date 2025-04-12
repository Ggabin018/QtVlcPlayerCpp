#include "file_manager.h"

fileManager::fileManager(const std::string& appName) : appName_(appName)
{}

std::filesystem::path fileManager::getConfigDir() {
#ifdef _WIN32
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, path))) {
        return std::filesystem::path(path) / appName_;
    }
#elif __APPLE__
    struct passwd* pw = getpwuid(getuid());
    return std::filesystem::path(pw->pw_dir) / "Library/Application Support" / appName_;
#else // Linux
    const char* xdgConfigHome = std::getenv("XDG_CONFIG_HOME");
    if (xdgConfigHome) {
        return std::filesystem::path(xdgConfigHome) / appName_;
    }
    struct passwd* pw = getpwuid(getuid());
    return std::filesystem::path(pw->pw_dir) / ".config" / appName_;
#endif
}

std::filesystem::path fileManager::getDataDir() {
#ifdef _WIN32
    char path[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, path))) {
        return std::filesystem::path(path) / appName_;
    }
#elif __APPLE__
    struct passwd* pw = getpwuid(getuid());
    return std::filesystem::path(pw->pw_dir) / "Library/Application Support" / appName_;
#else // Linux
    const char* xdgDataHome = std::getenv("XDG_DATA_HOME");
    if (xdgDataHome) {
        return std::filesystem::path(xdgDataHome) / appName_;
    }
    struct passwd* pw = getpwuid(getuid());
    return std::filesystem::path(pw->pw_dir) / ".local/share" / appName_;
#endif
}
