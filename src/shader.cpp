#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "shader.hpp"

using namespace std;

GLuint createShader(const string& path, GLenum type) {
    ifstream sourceFile;
    sourceFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    string sourceCode;
    
    try {
        sourceFile.open(path);
        stringstream buffer;
        buffer << sourceFile.rdbuf();
        sourceCode = buffer.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Error: could not read shader file " << path << ": " << e.what() << std::endl;
        glfwTerminate();
        exit(1);
    }

    const char* cStyleSourceCode = sourceCode.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &cStyleSourceCode, nullptr);
    glCompileShader(shader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Error when compiling shader at " << path << ": " << infoLog << std::endl;
        glfwTerminate();
        exit(1);
    }

    return shader;
}

Shader::Shader(const string &vertexShaderPath, const string &fragmentShaderPath) {
    const GLuint vertexShader = createShader(vertexShaderPath, GL_VERTEX_SHADER);
    const GLuint fragmentShader = createShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, sizeof(infoLog), nullptr, infoLog);
        std::cerr << "Error when linking shader program for " << vertexShaderPath << " and " << fragmentShaderPath
                << ": " << infoLog << std::endl;
        glfwTerminate();
        exit(1);
    }

    // Once linked, we don't need the shaders anymore
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() const {
    glUseProgram(ID);
}

GLint Shader::getUniformLocation(const char* uniformName) const {
    const GLint location = glGetUniformLocation(ID, uniformName);
    if (location == -1) {
        std::cerr << "Could not find uniform location for uniform: " << uniformName << std::endl;
        glfwTerminate();
        exit(1);
    }

    return location;
}

void Shader::setFloatUniform(const char* uniformName, float value) {
    glUniform1f(getUniformLocation(uniformName), value);
}

void Shader::setIntUniform(const char* uniformName, int value) {
    glUniform1i(getUniformLocation(uniformName), value);
}

void Shader::setMatrix4fUniform(const char* uniformName, glm::mat4 matrix) {
    glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3Uniform(const char* uniformName, float x, float y, float z) {
    glUniform3f(getUniformLocation(uniformName), x, y, z);
}

void Shader::setVec3Uniform(const char* uniformName, glm::vec3 vector) {
    glUniform3f(getUniformLocation(uniformName), vector.x, vector.y, vector.z);
}

void Shader::setVec4Uniform(const char* uniformName, float x, float y, float z, float w) {
    glUniform4f(getUniformLocation(uniformName), x, y, z, w);
}

void Shader::setVec4Uniform(const char* uniformName, glm::vec4 vector) {
    glUniform4f(getUniformLocation(uniformName), vector.x, vector.y, vector.z, vector.w);
}
