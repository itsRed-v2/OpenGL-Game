#include "inputs.hpp"

#include <iostream>

#include "math/raycast.hpp"

// Static event callback functions to forward event calls to the InputManager object

void EventCallbacks::onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->onKey(key, scancode, action, mods);
}

void EventCallbacks::onFrameBufferResize(GLFWwindow* window, int width, int height) {
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->onFrameBufferResize(width, height);
}

void EventCallbacks::onCursorMove(GLFWwindow* window, double newX, double newY) {
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->onCursorMove(newX, newY);
}

void EventCallbacks::onScroll(GLFWwindow* window, double offsetX, double offsetY) {
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->onScroll(offsetX, offsetY);
}

void EventCallbacks::onMouseButton(GLFWwindow* window, int button, int action, int mods) {
    static_cast<InputManager*>(glfwGetWindowUserPointer(window))->onMouseButton(button, action, mods);
}

// InputManager implementation

InputManager::InputManager(GLFWwindow* window, World &world, Camera &camera): window(window), world(world), camera(camera) {}

void InputManager::onKey(int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        swapInterval = swapInterval == 1 ? 0 : 1;
        glfwSwapInterval(swapInterval);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (cursorFree) {
            cursorFree = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            camera.syncCursorPosition(window);
        } else {
            cursorFree = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void InputManager::onFrameBufferResize(int width, int height) {
    glViewport(0, 0, width, height);
    camera.setFrameBufferSize(width, height);
}

void InputManager::onCursorMove(double newX, double newY) {
    if (cursorFree) return;

    camera.onCursorMove(newX, newY);
}

void InputManager::onScroll(double offsetX, double offsetY) {
    camera.onScroll(offsetX, offsetY);
}

void InputManager::onMouseButton(int button, int action, int mods) {
    if (action != GLFW_PRESS)
        return;

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        Ray camRay(camera.position, camera.getFrontVector());
        std::optional<HitResult> hit = world.rayCast(camRay);

        if (hit) {
            if (world.isInWorld(hit->blockPos)) {
                world.setBlock(hit->blockPos, Block::AIR);
            } else {
                std::cout << "Cannot break block outside of world !" << std::endl;
            }
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        Ray camRay(camera.position, camera.getFrontVector());
        std::optional<HitResult> hit = world.rayCast(camRay);

        if (hit) {
            Vec3i placePos = hit->blockPos.offset(hit->blockFace);
            if (world.isInWorld(placePos)) {
                world.setBlock(placePos, Block::SOLID);
            } else {
                std::cout << "Cannot place block outside of world !" << std::endl;
            }
        }
    }
}
