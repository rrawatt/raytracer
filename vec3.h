#ifndef VEC3_H
#define VEC3_H

#include <cmath>

struct vec3 {
    float x = 0, y = 0, z = 0;

    float& operator[](int i);
    const float& operator[](int i) const;

    vec3 operator*(float v) const;
    float operator*(const vec3& v) const;
    vec3 operator+(const vec3& v) const;
    vec3 operator-(const vec3& v) const;
    vec3 operator-() const;

    float norm() const;
    vec3 normalized() const;
};

vec3 cross(const vec3& v1, const vec3& v2);

#endif
