#include <filesystem>
#include <fstream>
#include <spdlog/spdlog.h>

std::string readFile(const std::string& path)
{
    if (!std::filesystem::exists(path)) {
        SPDLOG_ERROR("file does not exist: {}", path);
        throw std::runtime_error("file does not exist: " + path);
    }

    std::ifstream file(path);
    if (!file) {
        SPDLOG_ERROR("Failed to open file: {}", path);
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
