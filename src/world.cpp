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

Block World::getBlock(Vec3i pos) {
    Vec2i chunkCoordinate = blockPosToChunkPos(pos);
    // if no loaded chunk contains the pos OR if the pos is out of the vertical limits, return AIR by default
    if (chunks.count(chunkCoordinate) == 0 || pos.y < 0 || pos.y >= CHUNK_HEIGHT) {
        return Block::AIR;
    }

    Chunk &chunk = chunks.at(chunkCoordinate);
    int32_t x = pos.x % CHUNK_SIZE;
    if (x < 0) x += CHUNK_SIZE;
    int32_t z = pos.z % CHUNK_SIZE;
    if (z < 0) z += CHUNK_SIZE;

    return chunk.getBlock(x, pos.y, z);
}

HitResult World::rayCast(const Ray &ray) {
    glm::vec3 direction = ray.getDirection();
    glm::vec3 origin = ray.origin;

    char xSign = std::signbit(direction.x) ? -1 : 1;
    int32_t currentX = std::floor(origin.x);

    char ySign = std::signbit(direction.y) ? -1 : 1;
    int32_t currentY = std::floor(origin.y);

    char zSign = std::signbit(direction.z) ? -1 : 1;
    int32_t currentZ = std::floor(origin.z);

    for (int iteration = 0; iteration < 100; iteration++) {
        int32_t xPlane = xSign == 1 ? currentX + 1 : currentX;
        int32_t yPlane = ySign == 1 ? currentY + 1 : currentY;
        int32_t zPlane = zSign == 1 ? currentZ + 1 : currentZ;

        float tx = (xPlane - origin.x) / direction.x;
        float ty = (yPlane - origin.y) / direction.y;
        float tz = (zPlane - origin.z) / direction.z;

        if (tx < ty && tx < tz) currentX += xSign;
        else if (ty < tz) currentY += ySign;
        else currentZ += zSign;

        Vec3i blockPos(currentX, currentY, currentZ);
        if (getBlock(blockPos) == Block::SOLID) {
            return rayCubeIntersection(ray, blockPos);
        }
    }

    return { false, glm::vec3(), -1, Vec3i() };
}