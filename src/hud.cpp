#include "hud.hpp"

#define HUD_SCALE 5

Hud::Hud(GLFWwindow* window) {
    updateProjectionMatrix(window);

    shader.use();
    shader.setIntUniform("image", 1);

    // Setting up VAO for crosshair
    const float crosshairVertices[] = {
        0, 0, 0, 0, 0,
        1, 0, 0, 1, 0,
        0, 1, 0, 0, 1,
        0, 1, 0, 0, 1,
        1, 0, 0, 1, 0,
        1, 1, 0, 1, 1,
    };

    GLuint crosshairVBO;
    glGenVertexArrays(1, &crosshairVAO);
    glBindVertexArray(crosshairVAO);
    glGenBuffers(1, &crosshairVBO);
    glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVertices), crosshairVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
        reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Hud::updateProjectionMatrix(GLFWwindow* window) {
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float xScale, yScale;
    glfwGetWindowContentScale(window, &xScale, &yScale);

    projection = glm::scale(glm::mat4(1.0f), glm::vec3(
        HUD_SCALE * xScale / static_cast<float>(width),
        HUD_SCALE * yScale / static_cast<float>(height),
        1.0f
    ));
    shader.use();
    shader.setMatrix4fUniform("projection", projection);
}

void Hud::draw() {
    shader.use();

    glm::mat4 transform(1.0f);
    transform = glm::scale(transform, glm::vec3(16.0f));
    transform = glm::translate(transform, glm::vec3(-0.5, -0.5, 0.0));
    shader.setMatrix4fUniform("transform", transform);

    glBindVertexArray(crosshairVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}