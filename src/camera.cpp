#include <format>

#include "camera.hpp"

#include "logger.hpp"
#include "player.hpp"

#define ZOOM_SENSITIVITY 0.5f

#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

#define CAMERA_HEIGHT 1.6

Camera::Camera(GLFWwindow* window) {
    updateAspect(window);
}

void Camera::updateAspect(GLFWwindow* window) {
    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    aspect = static_cast<float>(frameBufferWidth) / static_cast<float>(frameBufferHeight);
}

void Camera::onScroll([[maybe_unused]] const double offsetX, const double offsetY) {
    baseFov -= static_cast<float>(offsetY) * ZOOM_SENSITIVITY;
    if (baseFov < 1.0f) baseFov = 1.0f;
    if (baseFov > 110.0f) baseFov = 110.0f;

    Logger::info(std::format("FOV: {:.2f}", baseFov));
}

void Camera::onMouseButton(const int button, const int action, [[maybe_unused]] const int mods) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        baseFov = DEFAULT_FOV;
    }
}

void Camera::update(const float deltaTime, const float tickDelta) {
    interpolatedPosition = getInterpolatedPosition(tickDelta);

    // TODO: use proper framerate-independent lerp smoothing
    const float targetFov = baseFov + fovOffset;
    const float diff = targetFov - effectiveFov;
    effectiveFov += diff * std::min(deltaTime * 8.0f, 1.0f);
}

void Camera::setPosition(const glm::vec3 newPosition) {
    lastPosition = position;
    position = newPosition;
}

void Camera::resetPosition(const glm::vec3 newPosition) {
    lastPosition = newPosition;
    position = newPosition;
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getInterpolatedPosition(const float tickDelta) const {
    return lastPosition * (1 - tickDelta) + position * tickDelta;
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(effectiveFov), aspect, NEAR_PLANE, FAR_PLANE);
}

glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view(1.0f);
    view = glm::rotate(view, glm::radians(pitch), glm::vec3(1.0, 0.0, 0.0));
    view = glm::rotate(view, glm::radians(yaw), glm::vec3(0.0, 1.0, 0.0));
    view = glm::translate(view, -interpolatedPosition);
    return view;
}

glm::vec3 Camera::getFrontVector() const {
    float radYaw = glm::radians(yaw);
    float radPitch = glm::radians(pitch);
    glm::vec3 dir(
        sin(radYaw) * cos(-radPitch),
        sin(-radPitch),
        - cos(radYaw) * cos(-radPitch)
    );
    return dir;
}