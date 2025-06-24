#ifndef FPS_COUNTER_HPP
#define FPS_COUNTER_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>

using namespace std;

class FpsCounter {
private:
    const float sampleDuration;
    float lastSampleStart;
    unsigned int currentFrameCount;
    unsigned int currentFPS;
    GLFWwindow* window;

public:
    FpsCounter(float sampleDuration, GLFWwindow* window);
    void tick();
};

#endif