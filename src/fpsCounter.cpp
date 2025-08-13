#include "fpsCounter.hpp"

FpsCounter::FpsCounter(GLFWwindow* window, const double sampleDuration) :
        window(window), sampleDuration(sampleDuration) {}

void FpsCounter::tick() {
    const double time = glfwGetTime();

    lastFrameDuration = time - lastFrameDate;
    lastFrameDate = time;

    if (time - lastSampleStart >= sampleDuration) {
        currentFPS = static_cast<unsigned int>(currentFrameCount / sampleDuration);
        currentFrameCount = 0;
        lastSampleStart = time;

        const double millisecondsPerFrame = 1000.0 / currentFPS;

        char windowTitle[40];
        snprintf(windowTitle, 40, "Voxels ! - FPS: %d / mspf: %.1f", currentFPS, millisecondsPerFrame);
        glfwSetWindowTitle(window, windowTitle);
    }

    currentFrameCount++;
}

double FpsCounter::getLastFrameDuration() const {
    return lastFrameDuration;
}
