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
    unsigned int currentFPS = 0;
    double lastFrameDate = glfwGetTime();
    double lastFrameDuration = 0.0f;

public:
    FpsCounter(GLFWwindow* window, double sampleDuration);
    void tick();

    [[nodiscard]] double getLastFrameDuration() const;
};

#endif