#include <glad/gl.h>

#include "fpsCounter.hpp"

FpsCounter::FpsCounter(float sampleDuration, GLFWwindow* window) : sampleDuration(sampleDuration), window(window) {
    currentFrameCount = 0;
}

void FpsCounter::tick() {
    float time = glfwGetTime();
    if (time - lastSampleStart >= sampleDuration) {
        currentFPS = currentFrameCount / sampleDuration;
        currentFrameCount = 0;
        lastSampleStart = time;

        char windowTitle[30];
        snprintf(windowTitle, 30, "Voxels ! - FPS: %d", currentFPS);
        glfwSetWindowTitle(window, windowTitle);
    }

    currentFrameCount++;
}