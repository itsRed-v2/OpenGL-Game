#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>

enum TextureType {
    DIFFUSE,
    SPECULAR
};

class Texture2D {
private:
    GLuint ID;

public:
    TextureType type;
    std::string path;

    Texture2D(std::string path, TextureType type, GLenum textureUnit);
    void bind(GLenum textureUnit);
};

#endif