#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>

#include "chunk.hpp"
#include "math/vectors.hpp"
#include "math/raycast.hpp"

class World {
public:
    unordered_map<Vec2i, Chunk> chunks;
    
    World();

    void draw(Shader &shader, GLuint cubeVAO);

    bool isInWorld(Vec3i pos) const;

    Block getBlock(Vec3i pos);
    void setBlock(Vec3i pos, Block block);
    
    std::optional<HitResult> rayCast(const Ray &ray);
};

#endif