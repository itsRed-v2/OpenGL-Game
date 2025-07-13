#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.hpp"
#include "fpsCounter.hpp"
#include "texture2D.hpp"
#include "camera.hpp"

#define ENABLE_VSYNC GLFW_TRUE

Camera* cameraPtr { NULL };
bool cursorFree = false;

void onKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    static int swapInterval = ENABLE_VSYNC;
    if (key == GLFW_KEY_V && action == GLFW_PRESS) {
        swapInterval = !swapInterval;
        glfwSwapInterval(swapInterval);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (cursorFree) {
            cursorFree = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            cameraPtr->syncCursorPosition(window);
        } else {
            cursorFree = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void onFrameBufferResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    if (cameraPtr == NULL) {
        std::cerr << "Error: cameraPtr is uninitialized." << std::endl;
        glfwTerminate();
        exit(1);
    }
    cameraPtr->setFrameBufferSize(width, height);
}

void onCursorMove(GLFWwindow* window, double newX, double newY) {
    if (cursorFree) return;

    if (cameraPtr == NULL) {
        std::cerr << "Error: cameraPtr is uninitialized." << std::endl;
        glfwTerminate();
        exit(1);
    }
    cameraPtr->onCursorMove(newX, newY);
}

void onScroll(GLFWwindow* window, double offsetX, double offsetY) {
    if (cameraPtr == NULL) {
        std::cerr << "Error: cameraPtr is uninitialized." << std::endl;
        glfwTerminate();
        exit(1);
    }
    cameraPtr->onScroll(offsetX, offsetY);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Error during GLFW initialization." << std::endl;
        return 1;
    }

    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "[GLFW Error]: " << description << std::endl;
    });

    // Creating a window
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

    // Loading OpenGL function pointers
    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    // Print useful info
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // Setting up OpenGL viewport and other settings
    int frameBufferWidth, frameBufferHeight;
    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(ENABLE_VSYNC ? 1 : 0);

    // Instantiate camera
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    Camera camera (cursorX, cursorY, frameBufferWidth, frameBufferHeight);
    cameraPtr = &camera;

    // Register GLFW callbacks and some settings
    glfwSetKeyCallback(window, onKey);
    glfwSetFramebufferSizeCallback(window, onFrameBufferResize);
    glfwSetCursorPosCallback(window, onCursorMove);
    glfwSetScrollCallback(window, onScroll);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    } else {
        std::cout << "[WARN] Raw mouse motion is not available on this system." << std::endl;
    }

    // Initializing some useful objects

    FpsCounter fpsCounter(0.5, window);

    Shader lightingShader ("assets/shaders/lighting.vs", "assets/shaders/lighting.fs");
    lightingShader.use();
    lightingShader.setVec3Uniform("lightColor", 1.0f, 1.0f, 1.0f);
    lightingShader.setVec3Uniform("objectColor", 1.0f, 0.5f, 0.31f);

    Shader lightCubeShader ("assets/shaders/light.vs", "assets/shaders/light.fs");

    float vertices[] = {
        // Positions         // Normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // Setting up VAO for object
    GLuint objectVAO;
    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Setting up VAO for light
    GLuint lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Setting scene up

    camera.position = glm::vec3(0.0, 1.0, 5.0);
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::mat4 projection, view, model;

    while (!glfwWindowShouldClose(window)) {
        float deltaTime = fpsCounter.getLastFrameDuration();
        camera.processInputs(window, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = camera.getProjectionMatrix();
        view = camera.getViewMatrix();
        
        // Rendering the light cube
        // lightPos = glm::vec3(1.2f, 1.0f, 2.0f + 1.0f * (float)glm::sin(glfwGetTime()));
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2));
        
        lightCubeShader.use();
        lightCubeShader.setMatrix4fUniform("projection", projection);
        lightCubeShader.setMatrix4fUniform("view", view);
        lightCubeShader.setMatrix4fUniform("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Rendering the object
        float angle = glfwGetTime() * 45.0;
        model = glm::mat4(1.0f);
        // model = glm::scale(model, glm::vec3(10.0f, 0.2f, 10.0f));
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
        
        lightingShader.use();
        lightingShader.setVec3Uniform("lightPos", lightPos);
        lightingShader.setMatrix4fUniform("projection", projection);
        lightingShader.setMatrix4fUniform("view", view);
        lightingShader.setMatrix4fUniform("model", model);

        glBindVertexArray(objectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
        fpsCounter.tick();
    }

    glfwTerminate();
    return 0;
}
