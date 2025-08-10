#include "vectors.hpp"

bool operator==(const Vec3i &a, const Vec3i &b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

bool operator==(const Vec2i &a, const Vec2i &b) {
    return a.x == b.x && a.y == b.y;
}

size_t std::hash<Vec3i>::operator()(const Vec3i &vec) const {
    hash<uint32_t> h;
    return ((h(vec.x) ^ (h(vec.y) << 1)) >> 1) ^ (h(vec.z) << 1);
}

size_t std::hash<Vec2i>::operator()(const Vec2i &vec) const {
    hash<uint32_t> h;
    return h(vec.x) ^ (h(vec.y) << 1);
}