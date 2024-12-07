#ifndef SCENE_H
#define SCENE_H

#include "vec3.h"
#include "material.h"

// Sphere structure declaration
struct Sphere {
    vec3 center;
    float radius;
    Material material;
};

extern const Sphere spheres[];
extern const vec3 lights[];

// Function declarations
vec3 cast_ray(const vec3 &orig, const vec3 &dir, int depth = 0);
std::tuple<bool, float> ray_sphere_intersect(const vec3 &orig, const vec3 &dir, const Sphere &s);
std::tuple<bool, vec3, vec3, Material> scene_intersect(const vec3 &orig, const vec3 &dir);

#endif // SCENE_H
