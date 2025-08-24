#include "player.hpp"

#include <format>

#include "math/aabb.hpp"
#include "world/block.hpp"
#include "math/direction.hpp"

#define MOVEMENT_SPEED 5.0f
#define MOUSE_SENSITIVITY 0.1f

#define BOX_WIDTH 0.7
#define BOX_HEIGHT 1.8
#define CAMERA_HEIGHT 1.6

Player::Player(Camera &camera, const glm::vec3 position): position(position), camera(camera) {
    camera.position = position + glm::vec3(0, CAMERA_HEIGHT, 0);
}

AABB Player::boundingBox() const {
    return {{
        position.x - BOX_WIDTH / 2,
        position.y,
        position.z - BOX_WIDTH / 2,
    }, {
        position.x + BOX_WIDTH / 2,
        position.y + BOX_HEIGHT,
        position.z + BOX_WIDTH / 2,
    }};
}

void Player::skipNextCursorMove() {
    shouldSkipNextCursorMove = true;
}

void Player::onCursorMove(const double newX, const double newY) {
    if (shouldSkipNextCursorMove) {
        shouldSkipNextCursorMove = false;
        cursorX = newX;
        cursorY = newY;
        return;
    }

    const double deltaX = newX - cursorX;
    const double deltaY = newY - cursorY;
    cursorX = newX;
    cursorY = newY;

    yaw += deltaX * MOUSE_SENSITIVITY;
    if (yaw < -180) yaw += 360;
    if (yaw > 180) yaw -= 360;

    pitch += deltaY * MOUSE_SENSITIVITY;
    if (pitch > 90) pitch = 90;
    if (pitch < -90) pitch = -90;

    camera.yaw = yaw;
    camera.pitch = pitch;
}

void Player::processMovement(GLFWwindow* window, const World &world, const float deltaTime) {
    glm::vec3 relativeVelocity(0.0, 0.0, 0.0);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        relativeVelocity.z -= MOVEMENT_SPEED;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        relativeVelocity.z += MOVEMENT_SPEED;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        relativeVelocity.x -= MOVEMENT_SPEED;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        relativeVelocity.x += MOVEMENT_SPEED;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        relativeVelocity.y += MOVEMENT_SPEED;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        relativeVelocity.y -= MOVEMENT_SPEED;

    // rotate the camera-relative velocity vector by yaw degrees around Y-axis to get world-relative movement
    const float radYaw = glm::radians(yaw);
    velocity = glm::vec3(
        relativeVelocity.x * cos(radYaw) - relativeVelocity.z * sin(radYaw),
        relativeVelocity.y,
        relativeVelocity.x * sin(radYaw) + relativeVelocity.z * cos(radYaw)
    );

    if (velocity != glm::vec3(0, 0, 0)) {
        moveWithCollisions(world, deltaTime);
        camera.position = position + glm::vec3(0.0, CAMERA_HEIGHT, 0.0);
    }
}

void Player::moveWithCollisions(const World &world, const float deltaTime) {
    const glm::vec3 movement = velocity * deltaTime;
    const Direction3D movementDir = Direction3D::fromVector(movement);
    const vector<AABB> collisionBoxes = gatherSurroundingCollisionBoxes(world, movement);

    for (int i = 0; i < 3; i++) { // For each axis
        float maxMovement = movement[i];
        if (maxMovement == 0)
            continue;

        const Axis axis = AXES[i];
        AABB playerBox = boundingBox();

        for (const AABB &box : collisionBoxes) {
            if (!box.intersectsAlongAxis(playerBox, axis)) continue;

            const float collisionFacePos = box.getFacePos(movementDir.getOppositeFace(axis));
            const float playerFacePos = playerBox.getFacePos(movementDir.getFace(axis));
            const float faceDistance = collisionFacePos - playerFacePos;

            if ((faceDistance < 1e-5 && faceDistance > -1e-5)
                        || (faceDistance > 1e-5 && maxMovement > faceDistance)
                        || (faceDistance < -1e-5 && maxMovement < faceDistance)) {
                maxMovement = faceDistance;
            }
        }

        position[i] += maxMovement;
    }

}

vector<AABB> Player::gatherSurroundingCollisionBoxes(const World &world, const glm::vec3 movement) const {
    const Vec3i currentBlockPos(position);
    const Vec3i finalBlockPos(position + movement);

    const int32_t xMin = std::min(currentBlockPos.x, finalBlockPos.x) - 1;
    const int32_t xMax = std::max(currentBlockPos.x, finalBlockPos.x) + 1;
    const int32_t yMin = std::min(currentBlockPos.y, finalBlockPos.y) - 1;
    const int32_t yMax = std::max(currentBlockPos.y, finalBlockPos.y) + 2;
    const int32_t zMin = std::min(currentBlockPos.z, finalBlockPos.z) - 1;
    const int32_t zMax = std::max(currentBlockPos.z, finalBlockPos.z) + 1;

    vector<AABB> collisionBoxes;
    for (auto x = xMin; x <= xMax; x++) {
        for (auto y = yMin; y <= yMax; y++) {
            for (auto z = zMin; z <= zMax; z++) {
                if (world.getBlock({x, y, z}) != Blocks::AIR)
                    collisionBoxes.push_back(AABB::ofBlock({x, y, z}));
            }
        }
    }
    return collisionBoxes;
}
