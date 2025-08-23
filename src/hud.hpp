#ifndef HUD_HPP
#define HUD_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"
#include "texturemanip/texture2D.hpp"

class Hud {
    Shader shader {"assets/shaders/hud.vert", "assets/shaders/hud.frag"};
    Texture2D crosshairTexture {"assets/textures/crosshair.png", GL_TEXTURE1};
    glm::mat4 projection {};
    GLuint crosshairVAO = 0;

public:
    Hud() = delete;
    explicit Hud(GLFWwindow* window);
    void updateProjectionMatrix(GLFWwindow* window);

    void draw();
};

#endif