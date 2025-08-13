#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

private:
    float yaw = 0.0f;
    float pitch = 0.0f;
    float fov = 45.0f;
    float aspect;
    glm::mat4 projection;

    double cursorX, cursorY;
    bool shouldSkipNextCursorMove = true;

    void updateProjectionMatrix();

public:
    explicit Camera(GLFWwindow* window);

    void skipNextCursorMove();
    void updateAspect(GLFWwindow* window);

    void onCursorMove(double newX, double newY);
    void onScroll(double offsetX, double offsetY);

    void processInputs(GLFWwindow* window, float deltaTime);

    [[nodiscard]] glm::mat4 getProjectionMatrix() const;
    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] glm::vec3 getFrontVector() const;
};

#endif