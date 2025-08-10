#include "world.hpp"

World::World() {
    for (int32_t x = -1; x < 2; x++) {
        for (int32_t z = -1; z < 2; z++) {
            Vec2i chunkCoordinate { x, z };
            Chunk chunk(chunkCoordinate);
            chunks.insert({ chunkCoordinate, chunk });
        }
    }
}

void World::draw(Shader &shader, GLint cubeVAO) {
    for (auto it = chunks.begin(); it != chunks.end(); it++) {
        it->second.draw(shader, cubeVAO);
    }
}