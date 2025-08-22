#ifndef VOXELS_ATLAS_HPP
#define VOXELS_ATLAS_HPP

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "texturemanip/texture2D.hpp"
#include "math/vectors.hpp"

struct TextureUV {
    Vec2i start;
    Vec2i size;

    TextureUV(int32_t x, int32_t y, int32_t w, int32_t h);
};

class Atlas {
    Texture2D texture;
    std::unordered_map<std::string, TextureUV> UVs;

public:
    Atlas(const std::string &path, GLenum textureUnit);
    void bindTexture(GLenum textureUnit) const;

    void registerTextureUV(const std::string &name, const TextureUV &textureUV);

    void applyTextureUV(glm::vec2 & textureCoordinate, const std::string &textureName) const;
};

#endif //VOXELS_ATLAS_HPP