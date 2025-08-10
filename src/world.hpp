#ifndef WORLD_HPP
#define WORLD_HPP

#include <unordered_map>

#include "chunk.hpp"
#include "math/vectors.hpp"

class World {
public:
    unordered_map<Vec2i, Chunk> chunks;
    
    World();
    void draw(Shader &shader, GLint cubeVAO);
};

#endif