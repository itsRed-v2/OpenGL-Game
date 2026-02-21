#include "fpsCounter.hpp"

#include <chrono>
#include <thread>

#define MAX_FPS 500
#define MINIMUM_FRAME_DURATION (1.0 / MAX_FPS)

FpsCounter::FpsCounter(GLFWwindow* window, TickCounter& tickCounter, const double sampleDuration) :
        window(window), tickCounter(tickCounter), sampleDuration(sampleDuration) {}

void FpsCounter::frameBegin() {
    using namespace std::this_thread;
    using namespace std::chrono;

    // Ensure FPS limit by sleeping the remaining amount of time.
    double time = glfwGetTime();
    const double lastFrameSpan = time - currentFrameBegin;
    if (lastFrameSpan < MINIMUM_FRAME_DURATION) {
        sleep_for(microseconds(static_cast<int64_t>((MINIMUM_FRAME_DURATION - lastFrameSpan) * 1000000)));
    }

    time = glfwGetTime();
    lastFrameDuration = time - currentFrameBegin;
    currentFrameBegin = time;
}

void FpsCounter::frameDone() {
    const double time = glfwGetTime();

    const double lastFrameDrawTime = time - currentFrameBegin;
    sampleTotalDrawTime += lastFrameDrawTime;
    sampleFrameCount++;

    if (time - currentSampleStart >= sampleDuration) {
        nextSample();
    }
}

void FpsCounter::nextSample() {
    const auto fps = static_cast<unsigned int>(sampleFrameCount / sampleDuration);
    const double avgMillisecondsPerFrame = 1000 * sampleTotalDrawTime / sampleFrameCount;
    const double tps = tickCounter.getTPS();
    const double mspt = tickCounter.getMSPT();

    char windowTitle[70];
    snprintf(windowTitle, 70, "Voxels ! - FPS: %d / mspf: %.2f / TPS: %.1f / mspt: %.2f",
             fps, avgMillisecondsPerFrame, tps, mspt);
    glfwSetWindowTitle(window, windowTitle);

    sampleFrameCount = 0;
    currentSampleStart = glfwGetTime();
    sampleTotalDrawTime = 0.0;
}

double FpsCounter::getLastFrameDuration() const {
    return lastFrameDuration;
}
