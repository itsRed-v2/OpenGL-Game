#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "texture2D.hpp"

Texture2D::Texture2D(const std::string &path, const GLenum textureUnit) {
    stbi_set_flip_vertically_on_load(true); // Because OpenGL interprets images upside down.

    int width, height, channelCount;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channelCount, 0);

    if (!data) {
        std::cerr << "Failed to load image at " << path << std::endl;
        glfwTerminate();
        exit(1);
    }

    if (channelCount != 3 && channelCount != 4) {
        std::cerr << "Unsupported channel count on image: " << channelCount << std::endl;
        glfwTerminate();
        exit(1);
    }

    int imageFormat = channelCount == 3 ? GL_RGB : GL_RGBA;

    glGenTextures(1, &ID);
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, imageFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
}

void Texture2D::bind(const GLenum textureUnit) const {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
