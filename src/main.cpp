#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.hpp"
#include "fpsCounter.hpp"
#include "texture2D.hpp"

#define ENABLE_VSYNC GLFW_TRUE

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    static int swapInterval = 1;
    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        swapInterval = !swapInterval;
        glfwSwapInterval(swapInterval);
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Error during GLFW initialization." << std::endl;
        return 1;
    }

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "[GLFW Error]: " << description << std::endl;
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Voxels !", NULL, NULL);
    if (!window) {
        std::cerr << "GLFW: window or OpenGL context creation failed" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, onKey);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glClearColor(0.2, 0.3, 0.3, 1.0);
    glfwSwapInterval(ENABLE_VSYNC ? 1 : 0);
    FpsCounter fpsCounter(0.5, window);

    Shader shader ("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
    shader.use();

    Texture2D containerTexture ("assets/textures/container.jpg", GL_TEXTURE0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    Texture2D faceTexture ("assets/textures/awesomeface.png", GL_TEXTURE1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    shader.setIntUniform("texture0", 0);
    shader.setIntUniform("texture1", 1);

    float vertices[] = {
        // position        // color         // Texture coord
         0.5,  0.5, 0.0,   1.0, 0.0, 0.0,   1.0, 1.0,
         0.5, -0.5, 0.0,   0.0, 1.0, 0.0,   1.0, 0.0,
        -0.5, -0.5, 0.0,   0.0, 0.0, 1.0,   0.0, 0.0,
        -0.5,  0.5, 0.0,   1.0, 1.0, 0.0,   0.0, 1.0,
    };
    unsigned int indices[] = {
        3, 0, 1,
        1, 2, 3
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    while (!glfwWindowShouldClose(window)) {
        float angle = glfwGetTime() * glm::pi<float>() / 2;
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0));
        trans = glm::rotate(trans, angle, glm::vec3(0.0, 0.0, 1.0));
        trans = glm::scale(trans, glm::vec3(0.5, 0.5, 1.0));

        shader.setMatrix4fUniform("transform", trans);

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
        fpsCounter.tick();
    }

    glfwTerminate();
    return 0;
}
