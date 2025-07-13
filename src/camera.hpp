#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;

private:
    float yaw;
    float pitch;
    float fov;

    double cursorX, cursorY;
    int frameBufferWidth, frameBufferHeight;

public:
    Camera(double initialCursorX, double initialCursorY, int frameBufferWidth, int frameBufferHeight);
    void processInputs(GLFWwindow* window, float deltaTime);

    void onCursorMove(double newX, double newY);
    void onScroll(double ossetX, double offsetY);

    void setFrameBufferSize(int frameBufferWidth, int frameBufferHeight);
    void syncCursorPosition(GLFWwindow* window);

    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
};

#endif