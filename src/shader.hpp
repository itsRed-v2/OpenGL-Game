#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

using namespace std;

class Shader {
    GLuint ID;

public:
    Shader(const string &vertexShaderPath, const string &fragmentShaderPath);

    void use() const;
    GLint getUniformLocation(const char* uniformName) const;

    void setFloatUniform(const char* uniformName, float value);
    void setIntUniform(const char* uniformName, int value);
    void setMatrix4fUniform(const char* uniformName, glm::mat4 matrix);
    void setVec3Uniform(const char* uniformName, float x, float y, float z);
    void setVec3Uniform(const char* uniformName, glm::vec3 vector);
    void setVec4Uniform(const char* uniformName, float x, float y, float z, float w);
    void setVec4Uniform(const char* uniformName, glm::vec4 vector);
};

#endif