#include "logger.hpp"

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void Logger::info(const std::string &msg) {
    std::cout << "[INFO] " << msg << std::endl;
}

void Logger::warn(const std::string &msg) {
    std::cerr << "[WARN] " << msg << std::endl;
}

void Logger::error(const std::string &msg) {
    std::cerr << "[ERROR] " << msg << std::endl;
}

void Logger::crash(const std::string &msg) {
    std::cerr << "The game has crashed !" << std::endl;
    std::cerr << msg << std::endl;
    glfwTerminate();
    exit(1);
}

