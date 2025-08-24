#ifndef VOXELS_DIRECTION_HPP
#define VOXELS_DIRECTION_HPP

#include <glm/glm.hpp>

#include "math/blockface.hpp"
#include "math/axis.hpp"

enum class Direction { POSITIVE, NEGATIVE };

struct Direction3D {
    Direction x = Direction::POSITIVE;
    Direction y = Direction::POSITIVE;
    Direction z = Direction::POSITIVE;

    static Direction3D fromVector(glm::vec3 vec);

    [[nodiscard]] BlockFace getFace(Axis axis) const;
    [[nodiscard]] BlockFace getOppositeFace(Axis axis) const;
};

#endif //VOXELS_DIRECTION_HPP