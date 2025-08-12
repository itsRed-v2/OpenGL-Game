#include <iostream>

#include "camera.hpp"

#define MOVEMENT_SPEED 10.0f
#define MOUSE_SENSITIVITY 0.1f
#define ZOOM_SENSITIVITY 0.5f

#define NEAR_PLANE 0.1f
#define FAR_PLANE 1000.0f

Camera::Camera(double initialCursorX, double initialCursorY, int frameBufferWidth, int frameBufferHeight)
        : cursorX(initialCursorX), cursorY(initialCursorY), frameBufferWidth(frameBufferWidth), frameBufferHeight(frameBufferHeight) {
    position = glm::vec3(0.0, 0.0, 0.0);
    yaw = 0.0f;
    pitch = 0.0f;
    fov = 45.0f;
}

void Camera::onCursorMove(double newX, double newY) {
    double deltaX = newX - cursorX;
    double deltaY = newY - cursorY;
    cursorX = newX;
    cursorY = newY;
    
    yaw += deltaX * MOUSE_SENSITIVITY;
    if (yaw < -180) yaw += 360;
    if (yaw > 180) yaw -= 360;

    pitch += deltaY * MOUSE_SENSITIVITY;
    if (pitch > 90) pitch = 90;
    if (pitch < -90) pitch = -90;

    // std::cout << yaw << "/" << pitch << std::endl;
}

void Camera::onScroll(double ossetX, double offsetY) {
    fov -= offsetY * ZOOM_SENSITIVITY;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 110.0f) fov = 110.0f;

    std::cout << "FOV: " << fov << std::endl;
}

void Camera::processInputs(GLFWwindow* window, float deltaTime) {
    glm::vec3 relativeMovement(0.0, 0.0, 0.0);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        relativeMovement.z -= MOVEMENT_SPEED * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        relativeMovement.z += MOVEMENT_SPEED * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        relativeMovement.x -= MOVEMENT_SPEED * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        relativeMovement.x += MOVEMENT_SPEED * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        relativeMovement.y += MOVEMENT_SPEED * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        relativeMovement.y -= MOVEMENT_SPEED * deltaTime;

    // rotate the camera-relative movement by yaw degrees around Y-axis to get world-relative movement
    float radYaw = glm::radians(yaw);
    glm::vec3 movement(
        relativeMovement.x * cos(radYaw) - relativeMovement.z * sin(radYaw),
        relativeMovement.y,
        relativeMovement.x * sin(radYaw) + relativeMovement.z * cos(radYaw)
    );

    position += movement;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        fov = 45.0f;
    }
}

glm::mat4 Camera::getProjectionMatrix() const {
    float aspect = static_cast<float>(frameBufferWidth) / static_cast<float>(frameBufferHeight);
    return glm::perspective(glm::radians(fov), aspect, NEAR_PLANE, FAR_PLANE);
}

glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view = glm::mat4(1.0f);
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

void Camera::setFrameBufferSize(int width, int height) {
    frameBufferWidth = width;
    frameBufferHeight = height;
}

void Camera::syncCursorPosition(GLFWwindow* window) {
    glfwGetCursorPos(window, &cursorX, &cursorY);
}