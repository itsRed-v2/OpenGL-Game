#ifndef TEXTURE_2D_HPP
#define TEXTURE_2D_HPP

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>

class Texture2D {
    GLuint ID;
    int width, height, channels;

public:
    Texture2D(const std::string &path, GLenum textureUnit);
    void bind(GLenum textureUnit) const;

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;
};

#endif