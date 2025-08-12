#include "inputs.hpp"

#include <iostream>

#include "math/intersection.hpp"

InputManager::InputManager(World &world, Camera &camera): world(world), camera(camera) {}

void InputManager::onClick(int button, int action, int mods) {
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
