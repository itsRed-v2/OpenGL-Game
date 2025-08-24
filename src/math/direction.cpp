#include "math/direction.hpp"

Direction3D Direction3D::fromVector(const glm::vec3 vec) {
    return {
        vec.x >= 0 ? Direction::POSITIVE : Direction::NEGATIVE,
        vec.y >= 0 ? Direction::POSITIVE : Direction::NEGATIVE,
        vec.z >= 0 ? Direction::POSITIVE : Direction::NEGATIVE,
    };
}

BlockFace Direction3D::getFace(const Axis axis) const {
    switch (axis) {
        case Axis::X:
            return x == Direction::POSITIVE ? BlockFace::EAST : BlockFace::WEST;
        case Axis::Y:
            return y == Direction::POSITIVE ? BlockFace::UP : BlockFace::DOWN;
        default:
            return z == Direction::POSITIVE ? BlockFace::SOUTH : BlockFace::NORTH;
    }
}

BlockFace Direction3D::getOppositeFace(const Axis axis) const {
    switch (axis) {
        case Axis::X:
            return x == Direction::POSITIVE ? BlockFace::WEST : BlockFace::EAST;
        case Axis::Y:
            return y == Direction::POSITIVE ? BlockFace::DOWN : BlockFace::UP;
        default:
            return z == Direction::POSITIVE ? BlockFace::NORTH : BlockFace::SOUTH;
    }
}

