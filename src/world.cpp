#include "world.hpp"

#include <stdexcept>

World::World() {
    // Setting up shaders
    chunkShader.use();
    chunkShader.setIntUniform("atlas", 0);
    highlightShader.use();
    highlightShader.setVec4Uniform("highlightColor", 1.0f, 0.7f, 0.0f, 0.25f);

    // Setting up VAO for the highlight cube
    constexpr float cubeVertices[] = {
        // Front face
        0, 0, 1, 0, 0,
        1, 0, 1, 1, 0,
        0, 1, 1, 0, 1,
        0, 1, 1, 0, 1,
        1, 0, 1, 1, 0,
        1, 1, 1, 1, 1,
        // Back face
        1, 0, 0, 0, 0,
        0, 0, 0, 1, 0,
        1, 1, 0, 0, 1,
        1, 1, 0, 0, 1,
        0, 0, 0, 1, 0,
        0, 1, 0, 1, 1,
        // Right face,
        1, 0, 1, 0, 0,
        1, 0, 0, 1, 0,
        1, 1, 1, 0, 1,
        1, 1, 1, 0, 1,
        1, 0, 0, 1, 0,
        1, 1, 0, 1, 1,
        // Left face
        0, 0, 0, 0, 0,
        0, 0, 1, 1, 0,
        0, 1, 0, 0, 1,
        0, 1, 0, 0, 1,
        0, 0, 1, 1, 0,
        0, 1, 1, 1, 1,
        // Top face
        0, 1, 1, 0, 0,
        1, 1, 1, 1, 0,
        0, 1, 0, 0, 1,
        0, 1, 0, 0, 1,
        1, 1, 1, 1, 0,
        1, 1, 0, 1, 1,
        // Bottom face,
        0, 0, 0, 0, 0,
        1, 0, 0, 1, 0,
        0, 0, 1, 0, 1,
        0, 0, 1, 0, 1,
        1, 0, 0, 1, 0,
        1, 0, 1, 1, 1,
    };

    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
        reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Instantiating chunks
    for (int32_t x = -1; x < 2; x++) {
        for (int32_t z = -1; z < 2; z++) {
            Vec2i chunkCoordinate { x, z };
            Chunk chunk(chunkCoordinate);
            chunks.insert({ chunkCoordinate, chunk });
        }
    }
}

void World::draw(const Camera &camera, const Atlas &atlas) {
    const glm::mat4 projection = camera.getProjectionMatrix();
    const glm::mat4 view = camera.getViewMatrix();

    // Chunk rendering
    chunkShader.use();
    chunkShader.setMatrix4fUniform("projection", projection);
    chunkShader.setMatrix4fUniform("view", view);

    for (auto &[pos, chunk] : chunks) {
        chunk.draw(chunkShader, atlas);
    }

    // Ray casting for selected cube highlight
    Ray camRay(camera.position, camera.getFrontVector());
    std::optional<HitResult> hit = rayCast(camRay);
    if (hit) {
        Vec3i blockHit = hit->blockPos;
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(blockHit.x, blockHit.y, blockHit.z));
        model = glm::translate(model, glm::vec3(0.5));
        model = glm::scale(model, glm::vec3(1.01));
        model = glm::translate(model, glm::vec3(-0.5));
        highlightShader.use();
        highlightShader.setMatrix4fUniform("projection", projection);
        highlightShader.setMatrix4fUniform("view", view);
        highlightShader.setMatrix4fUniform("model", model);

        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
}

bool World::isInWorld(Vec3i pos) const {
    Vec2i chunkCoordinate = blockPosToChunkPos(pos);
    return chunks.count(chunkCoordinate) == 1 && pos.y >= 0 && pos.y < CHUNK_HEIGHT;
}

block_id World::getBlock(Vec3i pos) const {
    if (!isInWorld(pos)) {
        return Blocks::AIR.id;
    }

    const Chunk &chunk = chunks.at(blockPosToChunkPos(pos));
    pos.x %= CHUNK_SIZE;
    if (pos.x < 0) pos.x += CHUNK_SIZE;
    pos.z %= CHUNK_SIZE;
    if (pos.z < 0) pos.z += CHUNK_SIZE;

    return chunk.getBlock(pos);
}

void World::setBlock(const Vec3i pos, const Block& block) {
    setBlock(pos, block.id);
}

void World::setBlock(Vec3i pos, const block_id id) {
    if (!isInWorld(pos)) {
        throw std::invalid_argument("Trying to set block outside of world");
    }

    Chunk &chunk = chunks.at(blockPosToChunkPos(pos));
    pos.x %= CHUNK_SIZE;
    if (pos.x < 0) pos.x += CHUNK_SIZE;
    pos.z %= CHUNK_SIZE;
    if (pos.z < 0) pos.z += CHUNK_SIZE;

    chunk.setBlock(pos, id);
}


std::optional<HitResult> World::rayCast(const Ray &ray) const {
    const glm::vec3 direction = ray.getDirection();
    const glm::vec3 origin = ray.origin;

    const char xSign = std::signbit(direction.x) ? -1 : 1;
    auto currentX = static_cast<int32_t>(std::floor(origin.x));

    const char ySign = std::signbit(direction.y) ? -1 : 1;
    auto currentY = static_cast<int32_t>(std::floor(origin.y));

    const char zSign = std::signbit(direction.z) ? -1 : 1;
    auto currentZ = static_cast<int32_t>(std::floor(origin.z));

    for (int iteration = 0; iteration < 100; iteration++) {
        const auto xPlane = static_cast<float>(xSign == 1 ? currentX + 1 : currentX);
        const auto yPlane = static_cast<float>(ySign == 1 ? currentY + 1 : currentY);
        const auto zPlane = static_cast<float>(zSign == 1 ? currentZ + 1 : currentZ);

        const float tx = (xPlane - origin.x) / direction.x;
        const float ty = (yPlane - origin.y) / direction.y;
        const float tz = (zPlane - origin.z) / direction.z;

        if (tx < ty && tx < tz) currentX += xSign;
        else if (ty < tz) currentY += ySign;
        else currentZ += zSign;

        Vec3i blockPos(currentX, currentY, currentZ);
        if (getBlock(blockPos) != Blocks::AIR) {
            return rayCubeIntersection(ray, blockPos);
        }
    }

    return std::nullopt;
}
