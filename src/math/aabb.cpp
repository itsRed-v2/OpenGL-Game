#include "math/aabb.hpp"

#include <algorithm>

AABB::AABB(const glm::vec3 min, const glm::vec3 max): min(min), max(max) {}

AABB AABB::ofBlock(Vec3i pos) {
    return {{pos.x, pos.y, pos.z}, {pos.x + 1, pos.y + 1, pos.z + 1}};
}

glm::vec3 AABB::size() const {
    return max - min;
}

float AABB::getFacePos(const BlockFace face) const {
    switch (face) {
        case BlockFace::EAST: return max.x;
        case BlockFace::WEST: return min.x;
        case BlockFace::UP: return max.y;
        case BlockFace::DOWN: return min.y;
        case BlockFace::SOUTH: return max.z;
        default: return min.z;
    }
}

AABB AABB::intersection(const AABB &other) const {
    return {{
        std::max(min.x, other.min.x),
        std::max(min.y, other.min.y),
        std::max(min.z, other.min.z)
    },{
        std::min(max.x, other.max.x),
        std::min(max.y, other.max.y),
        std::min(max.z, other.max.z)
    }};
}

bool AABB::intersectsAlongAxis(const AABB &other, const Axis axis) const {
    const glm::vec3 intersectionSize = intersection(other).size();

    if (axis == Axis::X) return intersectionSize.y > 1e-5 && intersectionSize.z > 1e-5;
    if (axis == Axis::Y) return intersectionSize.x > 1e-5 && intersectionSize.z > 1e-5;
    return intersectionSize.x > 1e-5 && intersectionSize.y > 1e-5;
}
