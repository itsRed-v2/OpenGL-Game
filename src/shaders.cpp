#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/gl.h>

#include "shaders.hpp"

using namespace std;

GLuint createShader(string filename, GLenum type) {
    ifstream sourceFile (filename);
    stringstream buffer;
    buffer << sourceFile.rdbuf();
    string sourceCode = buffer.str();
    const char* cStyleSourceCode = sourceCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &cStyleSourceCode, NULL);
    glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::cout << "Error when compiling shader at " << filename << ": " << infoLog << std::endl;
        glfwTerminate();
        exit(1);
    }

    return shader;
}

GLuint createProgram(string vertexShaderFile, string fragmentShaderFile) {
    GLuint vertexShader = createShader(vertexShaderFile, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragmentShaderFile, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, sizeof(infoLog), NULL, infoLog);
        std::cout << "Error when linking shader program for " << vertexShaderFile << " and " << fragmentShaderFile
                << ": " << infoLog << std::endl;
        glfwTerminate();
        exit(1);
    }

    // Once linked, we don't need the shaders anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}