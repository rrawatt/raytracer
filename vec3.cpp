#include "vec3.h"

float& vec3::operator[](int i) { return i == 0 ? x : (i == 1 ? y : z); }
const float& vec3::operator[](int i) const { return i == 0 ? x : (i == 1 ? y : z); }

vec3 vec3::operator*(float v) const { return {x * v, y * v, z * v}; }
float vec3::operator*(const vec3& v) const { return x * v.x + y * v.y + z * v.z; }
vec3 vec3::operator+(const vec3& v) const { return {x + v.x, y + v.y, z + v.z}; }
vec3 vec3::operator-(const vec3& v) const { return {x - v.x, y - v.y, z - v.z}; }
vec3 vec3::operator-() const { return {-x, -y, -z}; }

float vec3::norm() const { return std::sqrt(x * x + y * y + z * z); }
vec3 vec3::normalized() const { return (*this) * (1.f / norm()); }

vec3 cross(const vec3& v1, const vec3& v2) {
    return {v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x};
}
