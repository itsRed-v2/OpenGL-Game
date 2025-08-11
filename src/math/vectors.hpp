#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cstdint>
#include <unordered_map>

#include <glm/glm.hpp>

using namespace std;

struct Vec3i {
    int32_t x;
    int32_t y;
    int32_t z;

    Vec3i();
    Vec3i(int32_t x, int32_t y, int32_t z);
    Vec3i(float x, float y, float z);
    Vec3i(glm::vec3 pos);

    int32_t & operator[](size_t index);
    bool operator==(const Vec3i &other) const;
};

struct Vec2i {
    int32_t x;
    int32_t y;

    Vec2i();
    Vec2i(int32_t x, int32_t y);
    
    int32_t & operator[](size_t index);
    bool operator==(const Vec2i &other) const;
};

template<>
struct std::hash<Vec2i> {
    size_t operator()(const Vec2i &vec) const;
};

template<>
struct std::hash<Vec3i> {
    size_t operator()(const Vec3i &vec) const;
};

#endif