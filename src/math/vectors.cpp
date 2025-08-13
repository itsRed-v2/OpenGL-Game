#include "vectors.hpp"

#include <string>

Vec3i::Vec3i(const int32_t x, const int32_t y, const int32_t z): x(x), y(y), z(z) {}

bool Vec3i::operator==(const Vec3i &other) const {
    return x == other.x && y == other.y && z == other.z;
}

Vec3i Vec3i::offset(const BlockFace face) const {
    Vec3i copy(*this);
    switch (face)   {
        case BlockFace::UP:
            copy.y++;
            break;
        case BlockFace::DOWN:
            copy.y--;
            break;
        case BlockFace::EAST:
            copy.x++;
            break;
        case BlockFace::WEST:
            copy.x--;
            break;
        case BlockFace::SOUTH:
            copy.z++;
            break;
        case BlockFace::NORTH:
            copy.z--;
            break;
    }
    return copy;
}



Vec2i::Vec2i(const int32_t x, const int32_t y): x(x), y(y) {}

bool Vec2i::operator==(const Vec2i &other) const {
    return x == other.x && y == other.y;
}



size_t std::hash<Vec3i>::operator()(const Vec3i &vec) const noexcept {
    hash<uint32_t> h;
    return ((h(vec.x) ^ (h(vec.y) << 1)) >> 1) ^ (h(vec.z) << 1);
}

size_t std::hash<Vec2i>::operator()(const Vec2i &vec) const noexcept {
    hash<uint32_t> h;
    return h(vec.x) ^ (h(vec.y) << 1);
}
