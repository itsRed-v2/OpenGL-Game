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

        float millisecondsPerFrame = 1000.0 / currentFPS;

        char windowTitle[40];
        snprintf(windowTitle, 40, "Voxels ! - FPS: %d / mspf: %.1f", currentFPS, millisecondsPerFrame);
        glfwSetWindowTitle(window, windowTitle);
    }

    currentFrameCount++;
}