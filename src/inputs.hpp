#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "world.hpp"
#include "camera.hpp"
#include "hud.hpp"

namespace EventCallbacks {
    void onKey(GLFWwindow* window, int key, int scancode, int action, int mods);
    void onFrameBufferResize(GLFWwindow* window, int width, int height);
    void onCursorMove(GLFWwindow* window, double newX, double newY);
    void onScroll(GLFWwindow* window, double offsetX, double offsetY);
    void onMouseButton(GLFWwindow* window, int button, int action, int mods);
}

class InputManager {
    GLFWwindow* window;
    World &world;
    Camera &camera;
    Hud &hud;

    bool cursorFree = false;
    int swapInterval = 1;

public:
    InputManager() = delete;
    InputManager(GLFWwindow* window, World &world, Camera &camera, Hud &hud);

    void onKey(int key, int scancode, int action, int mods);
    void onFrameBufferResize(int width, int height);
    void onCursorMove(double newX, double newY);
    void onScroll(double offsetX, double offsetY);
    void onMouseButton(int button, int action, int mods);
};

#endif