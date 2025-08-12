#ifndef HUD_HPP
#define HUD_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "texture2D.hpp"

class Hud {
private:
    glm::mat4 projection;
    Shader shader;
    Texture2D crosshairTexture;
    GLuint crosshairVAO;

public:
    Hud() = delete;
    Hud(GLFWwindow* window);
    void updateProjectionMatrix(GLFWwindow* window);

    void draw();
};

#endif