#include "player.hpp"

#include <format>

#include "logger.hpp"
#include "fpsCounter.hpp"
#include "math/aabb.hpp"
#include "world/block.hpp"
#include "math/direction.hpp"

#define abs(x) ((x) >= 0 ? (x) : -(x))

#define MOUSE_SENSITIVITY 0.1f
#define DOUBLE_TAP_THRESHOLD 0.2

#define JUMPING_VELOCITY 9.0f
#define WALKING_SPEED 5.0f
#define RUNNING_SPEED (1.5f * WALKING_SPEED)
#define GROUND_COEFFICIENT 10.0f
#define AIR_COEFFICIENT 2.0f
#define VERTICAL_DRAG 0.25f
#define FLYING_VERTICAL_DRAG 8.0f
#define VERTICAL_FLYING_SPEED 80.0f
#define FLYING_SPEED_MULTIPLICATOR 3.0f
#define GRAVITY_STRENGTH 30.0f

#define BOX_WIDTH 0.7
#define BOX_HEIGHT 1.8
#define CAMERA_HEIGHT 1.6

Player::Player(Camera &camera, const glm::vec3 position): position(position), camera(camera) {
    camera.resetPosition(position + glm::vec3(0, CAMERA_HEIGHT, 0));
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

    yaw += static_cast<float>(deltaX) * MOUSE_SENSITIVITY;
    if (yaw < -180) yaw += 360;
    if (yaw > 180) yaw -= 360;

    pitch += static_cast<float>(deltaY) * MOUSE_SENSITIVITY;
    if (pitch > 90) pitch = 90;
    if (pitch < -90) pitch = -90;

    camera.yaw = yaw;
    camera.pitch = pitch;
}

void Player::onKey(const int key, const int action) {
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        if (glfwGetTime() - lastSpacePress < DOUBLE_TAP_THRESHOLD) {
            setFlying(!isFlying);
        }
        lastSpacePress = glfwGetTime();
    }
}

inline glm::vec3 Player::calculateUserAcceleration(GLFWwindow *window) const {
    // === Horizontal movement control ===

    // The camera-relative movement vector
    glm::vec3 relativeControl(0.0, 0.0, 0.0);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        relativeControl.z -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        relativeControl.z += 1.0f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        relativeControl.x -= 1.0f;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        relativeControl.x += 1.0f;

    // rotate the camera-relative vector by yaw degrees around Y-axis to get world-relative vector
    const float radYaw = glm::radians(yaw);
    glm::vec3 controlAcceleration(
        relativeControl.x * cos(radYaw) - relativeControl.z * sin(radYaw),
        0,
        relativeControl.x * sin(radYaw) + relativeControl.z * cos(radYaw)
    );
    if (controlAcceleration != glm::vec3(0, 0, 0)) {
        controlAcceleration = glm::normalize(controlAcceleration);

        float controlCoefficient = isOnGround ? GROUND_COEFFICIENT : AIR_COEFFICIENT;
        const bool running = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
        controlCoefficient *= running ? RUNNING_SPEED : WALKING_SPEED;
        if (isFlying) controlCoefficient *= FLYING_SPEED_MULTIPLICATOR;
        controlAcceleration *= controlCoefficient;
    }

    // === Vertical flying control ===

    if (isFlying) {
        float verticalControl = 0.0f;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            verticalControl += 1.0f;
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS
                || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
            verticalControl -= 1.0f;
        }
        controlAcceleration.y = verticalControl * VERTICAL_FLYING_SPEED;
    }

    return controlAcceleration;
}

inline glm::vec3 Player::calculateDragAcceleration() const {
    // TODO: revoir les coefficients pour plus de cohérence (AIR_COEFFICIENT != VERTICAL DRAG)
    const float horizontalDragCoef = isOnGround ? GROUND_COEFFICIENT : AIR_COEFFICIENT;
    const float verticalDragCoef = isFlying ? FLYING_VERTICAL_DRAG : VERTICAL_DRAG;
    // Note: Here drag cannot completely cancel velocity in a single tick because
    // all drag coefficients are less than the ticks per second (20TPS)
    return -velocity * glm::vec3(horizontalDragCoef, verticalDragCoef, horizontalDragCoef);
}

inline glm::vec3 Player::calculateGravityAcceleration() const {
    return isFlying ? glm::vec3(0.0f) : glm::vec3(0.0f, -GRAVITY_STRENGTH, 0.0f);
}

void Player::tickMovement(GLFWwindow* window, const World &world) {
    // Jumping handling
    if (isOnGround && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        velocity.y = JUMPING_VELOCITY;
        isOnGround = false;
    }

    // Acceleration calculations
    const glm::vec3 controlAcceleration = calculateUserAcceleration(window);
    const glm::vec3 drag = calculateDragAcceleration();
    const glm::vec3 gravity = calculateGravityAcceleration();
    velocity += (controlAcceleration + drag + gravity) * TICK_DURATION;

    // Movement calculations
    moveWithCollisions(world);
    camera.setPosition(position + glm::vec3(0.0, CAMERA_HEIGHT, 0.0));
}

void Player::moveWithCollisions(const World &world) {
    lastPosition = position;

    const glm::vec3 movement = velocity * TICK_DURATION;
    const Direction3D movementDir = Direction3D::fromVector(movement);
    const vector<AABB> collisionBoxes = gatherSurroundingCollisionBoxes(world, movement);

    // If player is on ground, the variable will be set by the collision check each frame.
    isOnGround = false;

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

            // Check if collision happened
            if ((faceDistance < 1e-5 && faceDistance > -1e-5)
                        || (faceDistance > 1e-5 && maxMovement > faceDistance)
                        || (faceDistance < -1e-5 && maxMovement < faceDistance)) {
                maxMovement = faceDistance;
                velocity[i] = 0; // Collision cancels speed in the axis of the collision

                // if player hits the ground
                if (axis == Axis::Y && movementDir.y == Direction::NEGATIVE) {
                    isOnGround = true;
                    setFlying(false);
                }
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

void Player::setFlying(const bool flying) {
    isFlying = flying;
    camera.fovOffset = flying ? +5.0f : 0.0f;
}