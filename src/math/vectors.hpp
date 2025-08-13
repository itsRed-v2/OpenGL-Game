#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cstdint>
#include <unordered_map>

#include "blockface.hpp"

using namespace std;

struct Vec3i {
    int32_t x;
    int32_t y;
    int32_t z;

    Vec3i() = delete;
    Vec3i(int32_t x, int32_t y, int32_t z);

    bool operator==(const Vec3i &other) const;

    [[nodiscard]] Vec3i offset(BlockFace face) const;
};

struct Vec2i {
    int32_t x;
    int32_t y;

    Vec2i() = delete;
    Vec2i(int32_t x, int32_t y);
    
    bool operator==(const Vec2i &other) const;
};

template<>
struct std::hash<Vec2i> {
    size_t operator()(const Vec2i &vec) const noexcept;
};

template<>
struct std::hash<Vec3i> {
    size_t operator()(const Vec3i &vec) const noexcept;
};

#endif