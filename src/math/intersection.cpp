#include "math/intersection.hpp"

#include <limits>
#include <algorithm>

Ray::Ray(glm::vec3 origin, glm::vec3 direction): origin(origin), direction(glm::normalize(direction)) {}

glm::vec3 Ray::getDirection() const {
    return direction;
}

HitResult rayCubeIntersection(const Ray &ray, Vec3i blockPos) {
    float tmin = 0, tmax = std::numeric_limits<float>::max();

    for (int i = 0; i < 3; i++) {
        int32_t boxMin = blockPos[i];
        int32_t boxMax = blockPos[i] + 1;
        float t1 = (boxMin - ray.origin[i]) / ray.getDirection()[i];
        float t2 = (boxMax - ray.origin[i]) / ray.getDirection()[i];

        tmin = std::max(tmin, std::min(t1, t2));
        tmax = std::min(tmax, std::max(t1, t2));
    }

    if (tmin < tmax && tmin > 0) {
        glm::vec3 hitPoint = ray.origin + (ray.getDirection() * tmin);
        return { true, hitPoint, tmin, blockPos };
    } else {
        return { false, glm::vec3(0, 0, 0), -1.0f, blockPos };
    }
}