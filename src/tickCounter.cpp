#include "tickCounter.hpp"

void TickCounter::tickBegin() {
    const double time = glfwGetTime();

    currentTickStart = currentTickStart + TICK_DURATION;
    // If ticking falls too much behind, skip missing ticks up to now.
    if (time - currentTickStart >= TICK_DURATION)
        currentTickStart = time;

    if (sampleTickCount >= TICKS_PER_SECOND) {
        // Average tick duration = time between a tick start and the following tick start (period between ticks)
        // In practice, this is roughly 50ms
        const double averageTickDuration = (time - sampleFirstTickStart) / sampleTickCount;
        tps = 1 / averageTickDuration;

        // Total calculation time: time actually spent processing ticking
        // In practice, this is much lower than 50ms
        mspt = 1000 * sampleTotalCalculationTime / sampleTickCount;

        sampleTickCount = 0;
        sampleTotalCalculationTime = 0.0;
        sampleFirstTickStart = time;
    }
}

void TickCounter::tickDone() {
    const double tickCalculationTime = glfwGetTime() - currentTickStart;
    sampleTotalCalculationTime += tickCalculationTime;
    sampleTickCount++;
}

bool TickCounter::shouldTick() const {
    return glfwGetTime() - currentTickStart >= TICK_DURATION;
}

double TickCounter::calculateTickDelta() const {
    return (static_cast<float>(glfwGetTime()) - currentTickStart) / TICK_DURATION;
}

double TickCounter::getTPS() const {
    return tps;
}

double TickCounter::getMSPT() const {
    return mspt;
}
