#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <optional>

#include <glm/glm.hpp>

#include "math/vectors.hpp"
#include "blockface.hpp"

class Ray {
private:
    const glm::vec3 direction;

public:
    const glm::vec3 origin;

    Ray(glm::vec3 origin, glm::vec3 direction);
    glm::vec3 getDirection() const;
};

struct HitResult {
    glm::vec3 hitPoint;
    float distance;
    Vec3i blockPos;
    BlockFace blockFace;
};

std::optional<HitResult> rayCubeIntersection(const Ray &ray, Vec3i blockPos);

#endif