#ifndef VOXELS_LOGGER_HPP
#define VOXELS_LOGGER_HPP

#include <string>

namespace Logger {
    void info(const std::string &msg);
    void warn(const std::string &msg);
    void error(const std::string &msg);
    [[noreturn]] void crash(const std::string &msg);
}

#endif //VOXELS_LOGGER_HPP