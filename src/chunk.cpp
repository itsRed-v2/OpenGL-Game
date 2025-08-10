#include "chunk.hpp"

Chunk::Chunk(Vec2i chunkCoordinate): chunkCoordinate(chunkCoordinate) {
    // Initialize the content array with air.
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                setBlock(x, y, z, Block::AIR);
            }
        }
    }

    // Make a floor
    for (int x = 1; x < CHUNK_SIZE - 1; x++) {
        for (int z = 1; z < CHUNK_SIZE - 1; z++) {
            setBlock(x, 10, z, Block::SOLID);
        }
    }
    setBlock(0, 9, 0, Block::SOLID);
    setBlock(CHUNK_SIZE - 1, 9, 0, Block::SOLID);
    setBlock(0, 9, CHUNK_SIZE - 1, Block::SOLID);
    setBlock(CHUNK_SIZE - 1, 9, CHUNK_SIZE - 1, Block::SOLID);
}

Block Chunk::getBlock(int x, int y, int z) {
    return content[x][z][y];
}

void Chunk::setBlock(int x, int y, int z, Block block) {
    content[x][z][y] = block;
}

void Chunk::draw(Shader &shader, GLint cubeVAO) {
    glBindVertexArray(cubeVAO);

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                Block block = getBlock(x, y, z);

                if (block == Block::SOLID) {
                    glm::mat4 model = glm::mat4(1.0f);
                    glm::vec3 translation(
                        x + chunkCoordinate.x * CHUNK_SIZE,
                        y,
                        z + chunkCoordinate.y * CHUNK_SIZE
                    );
                    model = glm::translate(model, translation);
                    shader.setMatrix4fUniform("model", model);

                    glDrawArrays(GL_TRIANGLES, 0, 36);
                }
            }
        }
    }

    glBindVertexArray(0);
}