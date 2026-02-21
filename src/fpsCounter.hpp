#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>

#include "tickCounter.hpp"

using namespace std;

class FpsCounter {
    GLFWwindow* window;
    TickCounter& tickCounter;
    const double sampleDuration;

    double currentSampleStart = glfwGetTime();
    unsigned int sampleFrameCount = 0;
    double currentFrameBegin = glfwGetTime();
    double lastFrameDuration = 0.0;
    double sampleTotalDrawTime = 0.0;

public:
    FpsCounter(GLFWwindow* window, TickCounter& tickCounter, double sampleDuration);
    void frameBegin();
    void frameDone();

    [[nodiscard]] double getLastFrameDuration() const;

private:
    void nextSample();
};

#endif