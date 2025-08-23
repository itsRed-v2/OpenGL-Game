#include "texturemanip/atlas.hpp"

#include <stdexcept>

#include "logger.hpp"

TextureUV::TextureUV(const int32_t x, const int32_t y, const int32_t w, const int32_t h) : start(x, y), size(w, h) {}

Atlas::Atlas(const std::string &path, const GLenum textureUnit): texture(path, textureUnit) {}

void Atlas::bindTexture(const GLenum textureUnit) const {
    texture.bind(textureUnit);
}

void Atlas::registerTextureUV(const std::string& name, const TextureUV &textureUV) {
    UVs.insert({name, textureUV});
}

void Atlas::applyTextureUV(glm::vec2& textureCoordinate, const std::string& textureName) const {
    try {
        const TextureUV& textureUV = UVs.at(textureName);

        textureCoordinate.x = (textureCoordinate.x * static_cast<float>(textureUV.size.x) + static_cast<float>(textureUV.start.x)) / static_cast<float>(texture.getWidth());

        float y = 1 - textureCoordinate.y;
        y = (y * static_cast<float>(textureUV.size.y) + static_cast<float>(textureUV.start.y)) / static_cast<float>(texture.getWidth());
        textureCoordinate.y = 1 - y;

    } catch ([[maybe_unused]] const std::out_of_range& o) {
        Logger::crash("No such texture in atlas: " + textureName);
    }
}
