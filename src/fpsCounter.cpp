#include "fpsCounter.hpp"

FpsCounter::FpsCounter(GLFWwindow* window, const double sampleDuration) :
        window(window), sampleDuration(sampleDuration) {}

void FpsCounter::frameBegin() {
    const double time = glfwGetTime();

    lastFrameDuration = time - lastFrameBegin;
    lastFrameBegin = time;
}

void FpsCounter::frameDone() {
    const double time = glfwGetTime();

    const double lastFrameDrawTime = time - lastFrameBegin;
    sampleTotalDrawTime += lastFrameDrawTime;
    currentFrameCount++;

    if (time - lastSampleStart >= sampleDuration) {
        const auto fps = static_cast<unsigned int>(currentFrameCount / sampleDuration);
        const double avgMillisecondsPerFrame = 1000 * sampleTotalDrawTime / currentFrameCount;

        char windowTitle[40];
        snprintf(windowTitle, 40, "Voxels ! - FPS: %d / mspf: %.2f", fps, avgMillisecondsPerFrame);
        glfwSetWindowTitle(window, windowTitle);

        currentFrameCount = 0;
        lastSampleStart = time;
        sampleTotalDrawTime = 0.0;
    }
}

double FpsCounter::getLastFrameDuration() const {
    return lastFrameDuration;
}
