#ifndef VOXELS_TICKCOUNTER_HPP
#define VOXELS_TICKCOUNTER_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#define TICKS_PER_SECOND 20
#define TICK_DURATION (1.0f / TICKS_PER_SECOND)

class TickCounter {
    double sampleFirstTickStart = glfwGetTime(); // Start time of first tick in the sample
    unsigned int sampleTickCount = 0; // Number of ticks finished since sample start
    double currentTickStart = glfwGetTime(); // Start of the current tick
    double sampleTotalCalculationTime = 0.0; // Total time spent ticking in this sample
    double tps = 0.0; // Ticks Per Second
    double mspt = 0.0; // Milliseconds Per Tick

public:
    void tickBegin();
    void tickDone();

    [[nodiscard]] bool shouldTick() const;
    [[nodiscard]] double calculateTickDelta() const;
    [[nodiscard]] double getTPS() const;
    [[nodiscard]] double getMSPT() const;

};


#endif //VOXELS_TICKCOUNTER_HPP