#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cstdint>
#include <unordered_map>

using namespace std;

struct Vec3i {
    int32_t x;
    int32_t y;
    int32_t z;
};

struct Vec2i {
    int32_t x;
    int32_t y;
};

bool operator==(const Vec3i &a, const Vec3i &b);
bool operator==(const Vec2i &a, const Vec2i &b);

template<>
struct std::hash<Vec2i> {
    size_t operator()(const Vec2i &vec) const;
};

template<>
struct std::hash<Vec3i> {
    size_t operator()(const Vec3i &vec) const;
};

#endif