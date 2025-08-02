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

    Texture2D containerDiffuseMap("assets/textures/container2.png", GL_TEXTURE0);
    Texture2D containerSpecularMap("assets/textures/container2_specular.png", GL_TEXTURE1);

    Shader lightingShader ("assets/shaders/lighting.vs", "assets/shaders/lighting.fs");
    lightingShader.use();
    lightingShader.setIntUniform("material.diffuse", 0);
    lightingShader.setIntUniform("material.specular", 1);
    lightingShader.setFloatUniform("material.shininess", 64.0f);

    Shader lightCubeShader ("assets/shaders/light.vs", "assets/shaders/light.fs");

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    // Setting up VAO for object
    GLuint objectVAO;
    glGenVertexArrays(1, &objectVAO);
    glBindVertexArray(objectVAO);

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
    
    // Setting up VAO for light
    GLuint lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Setting scene up

    camera.position = glm::vec3(0.0, 1.0, 5.0);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    }; 

    lightingShader.use();
    lightingShader.setVec3Uniform("directionalLight.direction", glm::normalize(glm::vec3(1.0, -1.0, 1.0)));
    lightingShader.setVec3Uniform("directionalLight.diffuse", 0.5, 1.0, 0.5);
    lightingShader.setVec3Uniform("directionalLight.specular", 0.5, 1.0, 0.5);

    lightingShader.setVec3Uniform("spotLight.diffuse", 1.0f, 0.5f, 0.5f);
    lightingShader.setVec3Uniform("spotLight.specular", 0.8f, 0.4f, 0.4f);
    lightingShader.setFloatUniform("spotLight.constant", 1.0f);
    lightingShader.setFloatUniform("spotLight.linear", 0.045f);
    lightingShader.setFloatUniform("spotLight.quadratic", 0.0075f);
    lightingShader.setFloatUniform("spotLight.innerCutoff", glm::cos(glm::radians(12.5f)));
    lightingShader.setFloatUniform("spotLight.outerCutoff", glm::cos(glm::radians(15.5f)));

    for (int i = 0; i < 4; i++) {
        string lightId = "pointLights[";
        lightId.push_back('0' + i);
        lightId.push_back(']');
        lightingShader.setVec3Uniform((lightId + ".position").c_str(), pointLightPositions[i]);
        lightingShader.setVec3Uniform((lightId + ".diffuse").c_str(), 0.5f, 0.5f, 1.0f);
        lightingShader.setVec3Uniform((lightId + ".specular").c_str(), 0.25f, 0.25f, 0.5f);
        lightingShader.setFloatUniform((lightId + ".constant").c_str(), 1.0f);
        lightingShader.setFloatUniform((lightId + ".linear").c_str(), 0.045f);
        lightingShader.setFloatUniform((lightId + ".quadratic").c_str(), 0.0075f);
    }

    glm::mat4 projection, view, model;

    while (!glfwWindowShouldClose(window)) {
        float deltaTime = fpsCounter.getLastFrameDuration();
        camera.processInputs(window, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = camera.getProjectionMatrix();
        view = camera.getViewMatrix();

        // Rendering the light cubes
        lightCubeShader.use();
        lightCubeShader.setMatrix4fUniform("projection", projection);
        lightCubeShader.setMatrix4fUniform("view", view);

        glBindVertexArray(lightCubeVAO);

        for (const glm::vec3 &pos : pointLightPositions) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::scale(model, glm::vec3(0.2));
            
            lightCubeShader.setMatrix4fUniform("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // Rendering the objects
        lightingShader.use();
        lightingShader.setMatrix4fUniform("projection", projection);
        lightingShader.setMatrix4fUniform("view", view);
        lightingShader.setVec3Uniform("camPos", camera.position);

        lightingShader.setVec3Uniform("spotLight.direction", camera.getFrontVector());
        lightingShader.setVec3Uniform("spotLight.position", camera.position);

        glBindVertexArray(objectVAO);

        for(unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lightingShader.setMatrix4fUniform("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
        fpsCounter.tick();
    }

    glfwTerminate();
    return 0;
}
