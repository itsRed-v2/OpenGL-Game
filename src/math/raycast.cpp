#include "math/raycast.hpp"

#include "logger.hpp"

#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

Ray::Ray(const glm::vec3 origin, const glm::vec3 direction): origin(origin), direction(glm::normalize(direction)) {}

glm::vec3 Ray::getDirection() const {
    return direction;
}

std::optional<HitResult> rayCubeIntersection(const Ray &ray, const Vec3i blockPos) {
    const glm::vec3 direction = ray.getDirection();
    const glm::vec3 origin = ray.origin;

    const float t1x = (blockPos.x - origin.x) / direction.x;
    const float t2x = (blockPos.x + 1 - origin.x) / direction.x;

    const float t1y = (blockPos.y - origin.y) / direction.y;
    const float t2y = (blockPos.y + 1 - origin.y) / direction.y;

    const float t1z = (blockPos.z - origin.z) / direction.z;
    const float t2z = (blockPos.z + 1 - origin.z) / direction.z;

    const float tmin = max(min(t1x, t2x), max(min(t1y, t2y), min(t1z, t2z)));
    const float tmax = min(max(t1x, t2x), min(max(t1y, t2y), max(t1z, t2z)));

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
    else Logger::crash("Could not determine block face in ray/cube intersection.");

    const glm::vec3 hitPoint = origin + (direction * tmin);
    return HitResult{ hitPoint, tmin, blockPos, blockFace };
}