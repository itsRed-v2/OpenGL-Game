#include <iostream>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"
#include "fpsCounter.hpp"
#include "camera.hpp"
#include "world.hpp"
#include "inputs.hpp"
#include "hud.hpp"
#include "texturemanip/atlas.hpp"

int main() {
    // Check block ID configuration
    ensureCorrectBlockIDs();

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

    GLFWwindow* window = glfwCreateWindow(960, 720, "Voxels !", nullptr, nullptr);
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
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSwapInterval(1);

    // Instantiate FPS counter
    FpsCounter fpsCounter(window, 0.5);

    // Instantiate camera
    Camera camera(window);
    camera.position = glm::vec3(0.0, 13.0, 0.0);

    // Instantiate world and hud
    World world;
    Hud hud(window);

    // Instantiate input manager
    InputManager input(window, world, camera, hud);
    glfwSetWindowUserPointer(window, &input);

    // Register GLFW callbacks
    glfwSetKeyCallback(window, EventCallbacks::onKey);
    glfwSetFramebufferSizeCallback(window, EventCallbacks::onFrameBufferResize);
    glfwSetCursorPosCallback(window, EventCallbacks::onCursorMove);
    glfwSetScrollCallback(window, EventCallbacks::onScroll);
    glfwSetMouseButtonCallback(window, EventCallbacks::onMouseButton);

    // Cursor settings
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported()) {
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    } else {
        std::cout << "[WARN] Raw mouse motion is not available on this system." << std::endl;
    }

    // Initializing opengl ressources

    Atlas atlas("assets/textures/atlas.png", GL_TEXTURE0);
    atlas.registerTextureUV("test", {0, 0, 16, 16});
    atlas.registerTextureUV("stone", {0, 16, 16, 16});
    atlas.registerTextureUV("grass_top", {16, 16, 16, 16});
    atlas.registerTextureUV("grass_sides", {16, 0, 16, 16});

    Shader cubeShader ("assets/shaders/lighting.vert", "assets/shaders/lighting.frag");
    cubeShader.use();
    cubeShader.setIntUniform("material.texture", 0);

    Shader highlightShader ("assets/shaders/highlight.vert", "assets/shaders/highlight.frag");
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
        fpsCounter.frameBegin();
        double deltaTime = fpsCounter.getLastFrameDuration();
        camera.processInputs(window, static_cast<float>(deltaTime));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = camera.getProjectionMatrix();
        view = camera.getViewMatrix();

        // Rendering the world
        cubeShader.use();
        cubeShader.setMatrix4fUniform("projection", projection);
        cubeShader.setMatrix4fUniform("view", view); 

        world.draw(cubeShader, atlas);

        // Ray casting
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

        glClear(GL_DEPTH_BUFFER_BIT); // We want hud elements to always be drawn on top of world elements

        hud.draw();

        fpsCounter.frameDone();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
