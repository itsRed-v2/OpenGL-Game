#include "chunk.hpp"

#include <stdexcept>
#include <unordered_map>
#include <chrono>
#include <iostream>

const unordered_map<BlockFace, array<float, 30>> quads = {
    { BlockFace::SOUTH, {
        0, 0, 1, 0, 0,
        1, 0, 1, 1, 0,
        0, 1, 1, 0, 1,
        0, 1, 1, 0, 1,
        1, 0, 1, 1, 0,
        1, 1, 1, 1, 1,
    }},
    { BlockFace::NORTH, {
        1, 0, 0, 0, 0,
        0, 0, 0, 1, 0,
        1, 1, 0, 0, 1,
        1, 1, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 1, 0, 1, 1,
    }},
    { BlockFace::EAST, {
        1, 0, 1, 0, 0,
        1, 0, 0, 1, 0,
        1, 1, 1, 0, 1,
        1, 1, 1, 0, 1,
        1, 0, 0, 1, 0,
        1, 1, 0, 1, 1,
    }},
    { BlockFace::WEST, {
        0, 0, 0, 0, 0,
        0, 0, 1, 1, 0,
        0, 1, 0, 0, 1,
        0, 1, 0, 0, 1,
        0, 0, 1, 1, 0,
        0, 1, 1, 1, 1,
    }},
    { BlockFace::UP, {
        0, 1, 1, 0, 0,
        1, 1, 1, 1, 0,
        0, 1, 0, 0, 1,
        0, 1, 0, 0, 1,
        1, 1, 1, 1, 0,
        1, 1, 0, 1, 1,
    }},
    { BlockFace::DOWN, {
        0, 0, 0, 0, 0,
        1, 0, 0, 1, 0,
        0, 0, 1, 0, 1,
        0, 0, 1, 0, 1,
        1, 0, 0, 1, 0,
        1, 0, 1, 1, 1,
    }}
};

Chunk::Chunk(const Vec2i chunkCoordinate): chunkCoordinate(chunkCoordinate) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Initialize the content array with air.
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                setBlock({x, y, z}, Block::AIR);
            }
        }
    }

    // Make a floor
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y <= 10; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                setBlock({x, y, z}, Block::SOLID);
            }
        }
    }

    setBlock({0, 11, 0}, Block::SOLID);
    setBlock({CHUNK_SIZE - 1, 11, 0}, Block::SOLID);
    setBlock({0, 11, CHUNK_SIZE - 1}, Block::SOLID);
    setBlock({CHUNK_SIZE - 1, 11, CHUNK_SIZE - 1}, Block::SOLID);
}

Block Chunk::getBlock(const Vec3i& pos) const {
    if (pos.x < 0 || pos.x >= CHUNK_SIZE
            || pos.y < 0 || pos.y >= CHUNK_HEIGHT
            || pos.z < 0 || pos.z >= CHUNK_SIZE)
        throw std::out_of_range("Block position out of range in chunk");
    
    return content[pos.x][pos.z][pos.y];
}

void Chunk::setBlock(const Vec3i& pos, const Block& block) {
    if (pos.x < 0 || pos.x >= CHUNK_SIZE
            || pos.y < 0 || pos.y >= CHUNK_HEIGHT
            || pos.z < 0 || pos.z >= CHUNK_SIZE)
        throw std::out_of_range("Block position out of range in chunk");

    content[pos.x][pos.z][pos.y] = block;
    recomputeMeshPending = true;
}

void Chunk::recomputeMesh() {
    using namespace std::chrono;
    auto start = high_resolution_clock::now();

    mesh.clear();

    for (int32_t x = 0; x < CHUNK_SIZE; x++) {
        for (int32_t y = 0; y < CHUNK_HEIGHT; y++) {
            for (int32_t z = 0; z < CHUNK_SIZE; z++) {
                Vec3i blockPos(x, y, z);
                Block block = getBlock(blockPos);
                if (block == Block::SOLID) {
                    for (auto [blockFace, quadVertices] : quads) {
                        Vec3i neighbor = blockPos.offset(blockFace);
                        if (neighbor.x < 0
                            || neighbor.x >= CHUNK_SIZE
                            || neighbor.y < 0
                            || neighbor.y >= CHUNK_HEIGHT
                            || neighbor.z < 0
                            || neighbor.z >= CHUNK_SIZE
                            || getBlock(neighbor) == Block::AIR)
                        {
                            for (int i = 0; i < 30; i += 5) {
                                quadVertices[i] += static_cast<float>(x);
                                quadVertices[i + 1] += static_cast<float>(y);
                                quadVertices[i + 2] += static_cast<float>(z);
                            }
                            mesh.insert(mesh.end(), quadVertices.begin(), quadVertices.end());
                        }
                    }
                }
            }
        }
    }

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), mesh.data(), GL_STATIC_DRAW);
    glBindVertexArray(0);

    auto end = high_resolution_clock::now();
    duration<double, std::milli> ms = end - start;
    std::cout << "Mesh building took " << ms.count() << "milliseconds" << std::endl;
}

void Chunk::draw(Shader &shader) {
    if (recomputeMeshPending) {
        recomputeMeshPending = false;
        recomputeMesh();
    }

    const glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(
        chunkCoordinate.x * CHUNK_SIZE,
        0,
        chunkCoordinate.y * CHUNK_SIZE
    ));
    shader.use();
    shader.setMatrix4fUniform("model", model);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, mesh.size() / 5);
    glBindVertexArray(0);
}

Vec2i blockPosToChunkPos(const Vec3i blockPos) {
    const int32_t chunkX = blockPos.x >= 0 ? (blockPos.x / CHUNK_SIZE) : ((blockPos.x + 1) / CHUNK_SIZE) - 1;
    const int32_t chunkZ = blockPos.z >= 0 ? (blockPos.z / CHUNK_SIZE) : ((blockPos.z + 1) / CHUNK_SIZE) - 1;
    return {chunkX, chunkZ};
}
