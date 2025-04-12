#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <filesystem>

#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#elif __APPLE__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#else
#include <cstdlib>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#endif

class fileManager
{
public:
    fileManager(const std::string& appName);
    std::filesystem::path getConfigDir();
    std::filesystem::path getDataDir();

private:
    const std::string appName_;
};

#endif // FILE_MANAGER_H
