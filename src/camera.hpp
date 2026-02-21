#ifndef CAMERA_HPP
#define CAMERA_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define DEFAULT_FOV 45.0f

class Camera {
public:
    float yaw = 0.0f;
    float pitch = 0.0f;
    float fovOffset = 0.0f;

private:
    float baseFov = DEFAULT_FOV;
    float effectiveFov = baseFov;
    float aspect = 1;

    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 lastPosition = position;
    glm::vec3 interpolatedPosition = position;

public:
    explicit Camera(GLFWwindow* window);

    void updateAspect(GLFWwindow* window);

    void onScroll(double offsetX, double offsetY);
    void onMouseButton(int button, int action, int mods);
    void update(float deltaTime, float tickDelta);

    void setPosition(glm::vec3 newPosition);
    void resetPosition(glm::vec3 newPosition);
    [[nodiscard]] glm::vec3 getPosition() const;
    [[nodiscard]] glm::vec3 getInterpolatedPosition(float tickDelta) const;

    [[nodiscard]] glm::mat4 getProjectionMatrix() const;
    [[nodiscard]] glm::mat4 getViewMatrix() const;
    [[nodiscard]] glm::vec3 getFrontVector() const;
};

#endif