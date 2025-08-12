#include "math/raycast.hpp"

#include <limits>
#include <algorithm>

Ray::Ray(glm::vec3 origin, glm::vec3 direction): origin(origin), direction(glm::normalize(direction)) {}

glm::vec3 Ray::getDirection() const {
    return direction;
}

std::optional<HitResult> rayCubeIntersection(const Ray &ray, Vec3i blockPos) {
    glm::vec3 direction = ray.getDirection();
    glm::vec3 origin = ray.origin;

    float t1x = (blockPos.x - origin.x) / direction.x;
    float t2x = (blockPos.x + 1 - origin.x) / direction.x;

    float t1y = (blockPos.y - origin.y) / direction.y;
    float t2y = (blockPos.y + 1 - origin.y) / direction.y;

    float t1z = (blockPos.z - origin.z) / direction.z;
    float t2z = (blockPos.z + 1 - origin.z) / direction.z;

    float tmin = std::max(std::min(t1x, t2x), std::max(std::min(t1y, t2y), std::min(t1z, t2z)));
    float tmax = std::min(std::max(t1x, t2x), std::min(std::max(t1y, t2y), std::max(t1z, t2z)));

    if (tmin < 0 || tmin > tmax) {
        return std::nullopt;
    }

    BlockFace blockFace;
    if (tmin == t1x) blockFace = BlockFace::WEST;
    else if (tmin == t2x) blockFace = BlockFace::EAST;
    else if (tmin == t1y) blockFace = BlockFace::DOWN;
    else if (tmin == t2y) blockFace = BlockFace::UP;
    else if (tmin == t1z) blockFace = BlockFace::NORTH;
    else if (tmin == t2z) blockFace = BlockFace::SOUTH;
    else throw runtime_error("Could not determine block face in ray/cube intersection");

    glm::vec3 hitPoint = origin + (direction * tmin);
    return HitResult{ hitPoint, tmin, blockPos, blockFace };
}