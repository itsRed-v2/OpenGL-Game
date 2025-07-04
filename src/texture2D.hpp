#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>

class Texture2D {
private:
    GLuint ID;

public:
    Texture2D(std::string path, GLenum textureUnit);
    void bind(GLenum textureUnit);
};

#endif