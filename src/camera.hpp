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
    float aspect;
    glm::mat4 projection;

    double cursorX, cursorY;

    void updateProjectionMatrix();

public:
    Camera(GLFWwindow* window);

    void syncCursorPosition(GLFWwindow* window);
    void updateAspect(GLFWwindow* window);

    void onCursorMove(double newX, double newY);
    void onScroll(double ossetX, double offsetY);

    void processInputs(GLFWwindow* window, float deltaTime);

    glm::mat4 getProjectionMatrix() const;
    glm::mat4 getViewMatrix() const;
    glm::vec3 getFrontVector() const;
};

#endif