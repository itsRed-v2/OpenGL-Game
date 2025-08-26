#ifndef VOXELS_PLAYER_HPP
#define VOXELS_PLAYER_HPP

#include "camera.hpp"
#include "math/aabb.hpp"
#include "world/world.hpp"

class Player {
    glm::vec3 position;
    glm::vec3 velocity = glm::vec3(0.0, 0.0, 0.0);
    float yaw = 0;
    float pitch = 0;

    Camera &camera;
    double cursorX = 0, cursorY = 0;
    bool shouldSkipNextCursorMove = true;

    bool isOnGround = false;
    bool isFlying = false;
    double lastSpacePress = 0.0f;

    [[nodiscard]] AABB boundingBox() const;

    void moveWithCollisions(const World &world, float deltaTime);
    [[nodiscard]] vector<AABB> gatherSurroundingCollisionBoxes(const World &world, glm::vec3 movement) const;

    [[nodiscard]] inline glm::vec3 calculateUserAcceleration(GLFWwindow* window) const;
    [[nodiscard]] inline glm::vec3 calculateDragAcceleration(float deltaTime) const;
    [[nodiscard]] inline glm::vec3 calculateGravityAcceleration() const;

    void setFlying(bool flying);

public:
    Player(Camera &camera, glm::vec3 position);

    void skipNextCursorMove();
    void onCursorMove(double newX, double newY);
    void onKey(int key, int action);
    void processMovement(GLFWwindow* window, const World &world, float deltaTime);
};

#endif //VOXELS_PLAYER_HPP