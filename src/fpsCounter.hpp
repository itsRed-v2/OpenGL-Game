#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>

using namespace std;

class FpsCounter {
    GLFWwindow* window;
    const double sampleDuration;
    double lastSampleStart = glfwGetTime();
    unsigned int currentFrameCount = 0;
    double lastFrameBegin = glfwGetTime();
    double lastFrameDuration = 0.0;
    double sampleTotalDrawTime = 0.0;

public:
    FpsCounter(GLFWwindow* window, double sampleDuration);
    void frameBegin();
    void frameDone();

    [[nodiscard]] double getLastFrameDuration() const;
};

#endif