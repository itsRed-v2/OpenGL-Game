#ifndef CHUNK_HPP
#define CHUNK_HPP

#include "shader.hpp"
#include "math/vectors.hpp"

#define CHUNK_SIZE 32
#define CHUNK_HEIGHT 64

enum Block {
    AIR, SOLID
};

class Chunk {
private:
    Vec2i chunkCoordinate;
    Block content[CHUNK_SIZE][CHUNK_SIZE][CHUNK_HEIGHT];

public:
    Chunk(Vec2i chunkCoordinate);
    Block getBlock(int x, int y, int z);
    void setBlock(int x, int y, int z, Block block);

    void draw(Shader &shader, GLint cubeVAO);
};

Vec2i blockPosToChunkPos(Vec3i blockPos);

#endif