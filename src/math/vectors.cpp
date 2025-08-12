#include "vectors.hpp"

#include <string>
#include <stdexcept>

Vec3i::Vec3i(): x(0), y(0), z(0) {}

Vec3i::Vec3i(int32_t x, int32_t y, int32_t z): x(x), y(y), z(z) {}

Vec3i::Vec3i(float x, float y, float z): x(std::floor(x)), y(std::floor(y)), z(std::floor(z)) {}

Vec3i::Vec3i(glm::vec3 pos): Vec3i(pos.x, pos.y, pos.z) {}

int32_t & Vec3i::operator[](size_t index) {
    switch (index) {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        default: throw std::out_of_range("Vec3i can only be indexed with values between 0 and 2, got " + std::to_string(index));
    }
}

bool Vec3i::operator==(const Vec3i &other) const {
    return x == other.x && y == other.y && z == other.z;
}

Vec3i Vec3i::offset(BlockFace face) {
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



Vec2i::Vec2i(): x(0), y(0) {}

Vec2i::Vec2i(int32_t x, int32_t y): x(x), y(y) {}

int32_t & Vec2i::operator[](size_t index) {
    switch (index) {
        case 0: return x;
        case 1: return y;
        default: throw std::out_of_range("Vec2i can only be indexed with values between 0 and 1, got " + std::to_string(index));
    }
}

bool Vec2i::operator==(const Vec2i &other) const {
    return x == other.x && y == other.y;
}



size_t std::hash<Vec3i>::operator()(const Vec3i &vec) const {
    hash<uint32_t> h;
    return ((h(vec.x) ^ (h(vec.y) << 1)) >> 1) ^ (h(vec.z) << 1);
}

size_t std::hash<Vec2i>::operator()(const Vec2i &vec) const {
    hash<uint32_t> h;
    return h(vec.x) ^ (h(vec.y) << 1);
}