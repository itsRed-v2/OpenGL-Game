#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "texture2D.hpp"

Texture2D::Texture2D(std::string path, GLenum textureUnit) {
    stbi_set_flip_vertically_on_load(true); // Because OpenGL interprets images upside down.

    int width, height, channelCount;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channelCount, 3); // We request 3 channels

    if (!data) {
        std::cerr << "Failed to load image at " << path << std::endl;
        glfwTerminate();
        exit(1);
    }

    glGenTextures(1, &ID);
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void Texture2D::bind(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
}