#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <glm/glm.hpp>

#include "math/vectors.hpp"

class Ray {
private:
    const glm::vec3 direction;

public:
    const glm::vec3 origin;

    Ray(glm::vec3 origin, glm::vec3 direction);
    glm::vec3 getDirection() const;
};

struct HitResult {
    bool success;
    glm::vec3 hitPoint;
    float distance;
    Vec3i blockHitPos;
};

HitResult rayCubeIntersection(const Ray &ray, Vec3i blockPos);

#endif