#include <format>

#include "camera.hpp"

#include "logger.hpp"

#define MOVEMENT_SPEED 10.0f
#define MOUSE_SENSITIVITY 0.1f
#define ZOOM_SENSITIVITY 0.5f

#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

Camera::Camera(GLFWwindow* window) {
    updateAspect(window);
}

void Camera::updateAspect(GLFWwindow* window) {
    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    aspect = static_cast<float>(frameBufferWidth) / static_cast<float>(frameBufferHeight);

    updateProjectionMatrix();
}

void Camera::updateProjectionMatrix() {
    projection = glm::perspective(glm::radians(fov), aspect, NEAR_PLANE, FAR_PLANE);
}

void Camera::onScroll([[maybe_unused]] const double offsetX, const double offsetY) {
    fov -= offsetY * ZOOM_SENSITIVITY;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 110.0f) fov = 110.0f;

    Logger::info(std::format("FOV: {:.2f}", fov));
    updateProjectionMatrix();
}

void Camera::onMouseButton(const int button, const int action, [[maybe_unused]] const int mods) {
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
        fov = 45.0f;
        updateProjectionMatrix();
    }
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projection;
}

glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view(1.0f);
    view = glm::rotate(view, glm::radians(pitch), glm::vec3(1.0, 0.0, 0.0));
    view = glm::rotate(view, glm::radians(yaw), glm::vec3(0.0, 1.0, 0.0));
    view = glm::translate(view, -position);
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