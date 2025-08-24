#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    float yaw = 0.0f;
    float pitch = 0.0f;

private:
    float fov = 45.0f;
    float aspect = 1;
    glm::mat4 projection {};

    void updateProjectionMatrix();

public:
    explicit Camera(GLFWwindow* window);

    void updateAspect(GLFWwindow* window);

    void onScroll(double offsetX, double offsetY);
    void onMouseButton(int button, int action, int mods);

    [[nodiscard]] glm::mat4 getProjectionMatrix() const;
    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] glm::vec3 getFrontVector() const;
};

#endif