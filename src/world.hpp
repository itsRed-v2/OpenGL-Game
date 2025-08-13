#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>

#include "chunk.hpp"
#include "math/vectors.hpp"
#include "math/raycast.hpp"

class World {
    unordered_map<Vec2i, Chunk> chunks;

public:
    World();

    void draw(Shader &shader);

    [[nodiscard]] bool isInWorld(Vec3i pos) const;

    [[nodiscard]] Block getBlock(Vec3i pos) const;
    void setBlock(Vec3i pos, Block block);
    
    [[nodiscard]] std::optional<HitResult> rayCast(const Ray &ray) const;
};

#endif