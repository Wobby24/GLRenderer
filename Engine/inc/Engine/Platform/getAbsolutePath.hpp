#pragma

#include <filesystem>
#include <stdexcept>
#include <iostream>

#include <string>
#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable: 4005)  // Disable macro redefinition warning
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(pop)
#else
#include <unistd.h>
#include <limits.h>
#endif


namespace Renderer {
    inline std::filesystem::path getExecutableDir() {
#ifdef _WIN32
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        return std::filesystem::path(buffer).parent_path();

#elif __APPLE__
        char buffer[1024];
        uint32_t size = sizeof(buffer);
        if (_NSGetExecutablePath(buffer, &size) == 0) {
            return std::filesystem::path(buffer).parent_path();
        }
        return {}; // Failed

#elif __linux__
        char buffer[1024];
        ssize_t count = readlink("/proc/self/exe", buffer, sizeof(buffer));
        if (count != -1) {
            return std::filesystem::path(std::string(buffer, count)).parent_path();
        }
        return {}; // Failed

#else
        return {}; // Unsupported platform
#endif
    }

    inline std::string getAbsoluteResourcePath(const std::string& relativeResourcePath, const std::string& targetResourceDir) {
        auto exeDir = getExecutableDir();

        std::filesystem::path resourceRoot;
        std::filesystem::path searchDir = exeDir;
        bool found = false;

        for (int i = 0; i < 6; ++i) {
            auto candidate = searchDir / targetResourceDir;
            bool exists = std::filesystem::exists(candidate);
            if (exists) {
                resourceRoot = candidate.lexically_normal();
                found = true;
                break;
            }
            searchDir = searchDir.parent_path();
            if (searchDir == searchDir.root_path())
                break;
        }

        if (!found) {
            throw std::runtime_error("Could not locate " + targetResourceDir + " folder relative to executable");
        }

        std::filesystem::path inputPath(relativeResourcePath);
        std::filesystem::path relativePath;

        // Convert inputPath to absolute path
        auto absoluteInput = std::filesystem::absolute(inputPath).lexically_normal();

        // Check if absoluteInput starts with resourceRoot path
        if (absoluteInput.string().find(resourceRoot.string()) == 0) {
            try {
                relativePath = std::filesystem::relative(absoluteInput, resourceRoot);
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cerr << "[Warning] std::filesystem::relative failed: " << e.what() << std::endl;
                relativePath = inputPath; // fallback
            }
        }
        else {
            relativePath = inputPath;
        }

        auto fullPath = resourceRoot / relativePath;
        fullPath = fullPath.lexically_normal();

        return fullPath.string();
    }

}

