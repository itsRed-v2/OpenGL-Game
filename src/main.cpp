#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "fpsCounter.hpp"
#include "texture2D.hpp"
#include "camera.hpp"
#include "chunk.hpp"
#include "world.hpp"
#include "inputs.hpp"

#define ENABLE_VSYNC GLFW_TRUE

Camera* cameraPtr { NULL };
InputManager* inputManagerPtr { NULL };
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

void onMouseButton(GLFWwindow* window, int button, int action, int mods) {
    if (inputManagerPtr == NULL) {
        std::cerr << "Error: inputManagerPtr is uninitialized." << std::endl;
        glfwTerminate();
        exit(1);
    }
    inputManagerPtr->onClick(button, action, mods);
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

    GLFWwindow* window = glfwCreateWindow(960, 720, "Voxels !", NULL, NULL);
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(ENABLE_VSYNC ? 1 : 0);

    // Instantiate camera
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);
    Camera camera (cursorX, cursorY, frameBufferWidth, frameBufferHeight);
    cameraPtr = &camera;
    
    camera.position = glm::vec3(0.0, 12.0, 0.0);

    // Instantiate world
    World world;

    // Instantiate input manager
    InputManager input(world, camera);
    inputManagerPtr = &input;

    // Instantiate FPS counter
    FpsCounter fpsCounter(0.5, window);

    // Register GLFW callbacks and some settings
    glfwSetKeyCallback(window, onKey);
    glfwSetFramebufferSizeCallback(window, onFrameBufferResize);
    glfwSetCursorPosCallback(window, onCursorMove);
    glfwSetScrollCallback(window, onScroll);
    glfwSetMouseButtonCallback(window, onMouseButton);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    } else {
        std::cout << "[WARN] Raw mouse motion is not available on this system." << std::endl;
    }

    // Initializing opengl ressources

    Texture2D testTexture("assets/textures/test-16px.png", GL_TEXTURE0);

    Shader cubeShader ("assets/shaders/lighting.vs", "assets/shaders/lighting.fs");
    cubeShader.use();
    cubeShader.setIntUniform("material.texture", 0);

    Shader highlightShader ("assets/shaders/highlight.vs", "assets/shaders/highlight.fs");
    highlightShader.use();
    highlightShader.setVec4Uniform("highlightColor", 1.0f, 0.7f, 0.0f, 0.25f);

    float vertices[] = {
        // Front face
        0, 0, 1, 0, 0,
        1, 0, 1, 1, 0,
        0, 1, 1, 0, 1,
        0, 1, 1, 0, 1,
        1, 0, 1, 1, 0,
        1, 1, 1, 1, 1,
        // Back face
        1, 0, 0, 0, 0,
        0, 0, 0, 1, 0,
        1, 1, 0, 0, 1,
        1, 1, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 1, 0, 1, 1,
        // Right face,
        1, 0, 1, 0, 0,
        1, 0, 0, 1, 0,
        1, 1, 1, 0, 1,
        1, 1, 1, 0, 1,
        1, 0, 0, 1, 0,
        1, 1, 0, 1, 1,
        // Left face
        0, 0, 0, 0, 0,
        0, 0, 1, 1, 0,
        0, 1, 0, 0, 1,
        0, 1, 0, 0, 1,
        0, 0, 1, 1, 0,
        0, 1, 1, 1, 1,
        // Top face
        0, 1, 1, 0, 0,
        1, 1, 1, 1, 0,
        0, 1, 0, 0, 1,
        0, 1, 0, 0, 1,
        1, 1, 1, 1, 0,
        1, 1, 0, 1, 1,
        // Bottom face,
        0, 0, 0, 0, 0,
        1, 0, 0, 1, 0,
        0, 0, 1, 0, 1,
        0, 0, 1, 0, 1,
        1, 0, 0, 1, 0,
        1, 0, 1, 1, 1,
    };

    // Setting up VAO for cube
    GLuint cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glm::mat4 projection, view, model;

    while (!glfwWindowShouldClose(window)) {
        float deltaTime = fpsCounter.getLastFrameDuration();
        camera.processInputs(window, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = camera.getProjectionMatrix();
        view = camera.getViewMatrix();

        // Rendering the world
        cubeShader.use();
        cubeShader.setMatrix4fUniform("projection", projection);
        cubeShader.setMatrix4fUniform("view", view); 

        world.draw(cubeShader, cubeVAO);

        // Raycasting
        Ray camRay(camera.position, camera.getFrontVector());
        std::optional<HitResult> hit = world.rayCast(camRay);
        if (hit) {
            glBindVertexArray(cubeVAO);
            Vec3i blockHit = hit->blockPos;
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(blockHit.x, blockHit.y, blockHit.z));
            model = glm::translate(model, glm::vec3(0.5));
            model = glm::scale(model, glm::vec3(1.01));
            model = glm::translate(model, glm::vec3(-0.5));
            highlightShader.use();
            highlightShader.setMatrix4fUniform("projection", projection);
            highlightShader.setMatrix4fUniform("view", view); 
            highlightShader.setMatrix4fUniform("model", model); 
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        fpsCounter.tick();
    }

    glfwTerminate();
    return 0;
}
