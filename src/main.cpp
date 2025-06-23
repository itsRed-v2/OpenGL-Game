#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "shaders.hpp"

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
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
    glfwSwapInterval(1);

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    glClearColor(0.2, 0.3, 0.3, 1.0);

    GLuint rectangleProgram = createProgram("assets/vertex.glsl", "assets/rectFragment.glsl");
    GLuint triangleProgram = createProgram("assets/vertex.glsl", "assets/triFragment.glsl");

    float rect_vertices[] = {
        -0.5, 0.5, 0,
        0.5, 0.5, 0,
        0.5, -0.5, 0,
        -0.5, -0.5, 0
    };
    unsigned int rect_indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLuint rect_VAO;
    glGenVertexArrays(1, &rect_VAO);
    glBindVertexArray(rect_VAO);

    GLuint rect_VBO;
    glGenBuffers(1, &rect_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, rect_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    GLuint rect_EBO;
    glGenBuffers(1, &rect_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rect_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rect_indices), rect_indices, GL_STATIC_DRAW);


    float triangle_vertices[] = {
        0, 0.75, 0,
        0.25, 0, 0,
        -0.25, 0, 0
    };

    GLuint tri_VAO;
    glGenVertexArrays(1, &tri_VAO);
    glBindVertexArray(tri_VAO);

    GLuint tri_VBO;
    glGenBuffers(1, &tri_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, tri_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(rectangleProgram);
        glBindVertexArray(rect_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glUseProgram(triangleProgram);
        glBindVertexArray(tri_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
