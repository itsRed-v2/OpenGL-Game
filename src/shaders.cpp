#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/gl.h>

#include "shaders.hpp"

using namespace std;

GLuint createShader(string path, GLenum type) {
    ifstream sourceFile;
    sourceFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    string sourceCode;
    
    try {
        sourceFile.open(path);
        stringstream buffer;
        buffer << sourceFile.rdbuf();
        sourceCode = buffer.str();
    } catch (std::ifstream::failure e) {
        std::cerr << "Error: could not read shader file " << path << std::endl;
        glfwTerminate();
        exit(1);
    }

    const char* cStyleSourceCode = sourceCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &cStyleSourceCode, NULL);
    glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), NULL, infoLog);
        std::cerr << "Error when compiling shader at " << path << ": " << infoLog << std::endl;
        glfwTerminate();
        exit(1);
    }

    return shader;
}

Shader::Shader(string vertexShaderPath, string fragmentShaderPath) {
    GLuint vertexShader = createShader(vertexShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, sizeof(infoLog), NULL, infoLog);
        std::cerr << "Error when linking shader program for " << vertexShaderPath << " and " << fragmentShaderPath
                << ": " << infoLog << std::endl;
        glfwTerminate();
        exit(1);
    }

    // Once linked, we don't need the shaders anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(ID);
}

GLint Shader::getUniformLocation(const char* uniformName) {
    GLint location = glGetUniformLocation(ID, uniformName);
    if (location == -1) {
        std::cerr << "Could not find uniform location for uniform: " << uniformName << std::endl;
        glfwTerminate();
        exit(1);
    }

    return location;
}