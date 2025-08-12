#ifndef INPUTS_HPP
#define INPUTS_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "world.hpp"
#include "camera.hpp"

class InputManager {
private:
    World &world;
    Camera &camera;

public:
    InputManager() = delete;
    InputManager(World &world, Camera &camera);
    void onClick(int button, int action, int mods);
};

#endif