#ifndef VOXELS_AABB_HPP
#define VOXELS_AABB_HPP

#include <glm/glm.hpp>

#include <math/vectors.hpp>

#include "axis.hpp"

class AABB {
public:
    glm::vec3 min;
    glm::vec3 max;

    AABB(glm::vec3 min, glm::vec3 max);
    [[nodiscard]] static AABB ofBlock(Vec3i pos);

    [[nodiscard]] glm::vec3 size() const;
    [[nodiscard]] float getFacePos(BlockFace face) const;

    [[nodiscard]] AABB intersection(const AABB& other) const;
    [[nodiscard]] bool intersectsAlongAxis(const AABB& other, Axis axis) const;
};


#endif //VOXELS_AABB_HPP