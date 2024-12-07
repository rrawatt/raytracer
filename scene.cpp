#include "scene.h"
#include <cmath>
#include <algorithm>

// Define the spheres in the scene
const Sphere spheres[] = {
    // Large central mirror ball
             // Large mirror sphere at the center

    {{ 10,  10, -40 }, 3, metal},             // Small ivory sphere
    {{-10,  12, -60 }, 1, glass},             // Small glass sphere
    {{ 5, -15, -45 }, 4, trans},         // Small red rubber sphere
    {{-12, -10, -75 }, 1, trans},             // Small translucent sphere
    {{ 15,  5, -70 }, 1, metal},              // Small metal sphere
    {{-8,  8, -55 }, 5, mirror},              // Small mirror sphere
    {{ 7, -12, -85 }, 1, metal},              // Small ivory sphere
    {{-5,  6, -90 }, 2, glass},               // Small glass sphere
    {{ 13, -7, -100 }, 1, trans},        // Small red rubber sphere
    {{-9, -15, -110 }, 2, trans},             // Small translucent sphere
    {{ 4,  15, -120 }, 1, metal},
    {{ 0, 0, -50 }, 8, mirror}              // Small metal sphere
};


// Define the lights in the scene
const vec3 lights[] = {
    {-15, 25,  10},
    { 40, 45, -30},
    { 25, 15,  35}
};

// Function to compute ray-sphere intersection
std::tuple<bool, float> ray_sphere_intersect(const vec3 &orig, const vec3 &dir, const Sphere &s) {
    vec3 L = s.center - orig;
    float tca = L * dir;
    float d2 = L * L - tca * tca;
    if (d2 > s.radius * s.radius) return {false, 0};
    float thc = std::sqrt(s.radius * s.radius - d2);
    float t0 = tca - thc, t1 = tca + thc;
    if (t0 > .001) return {true, t0};
    if (t1 > .001) return {true, t1};
    return {false, 0};
}

// Function to find the closest intersection point in the scene
std::tuple<bool, vec3, vec3, Material> scene_intersect(const vec3 &orig, const vec3 &dir) {
    vec3 pt, N;
    Material material;

    float nearest_dist = 1e10;
    if (std::abs(dir.y) > .001) { // Intersect with the checkerboard plane
        float d = -(orig.y + 4) / dir.y;
        vec3 p = orig + dir * d;
        if (d > .001 && d < nearest_dist && std::abs(p.x) < 10 && p.z < -10 && p.z > -30) {
            nearest_dist = d;
            pt = p;
            N = {0, 1, 0};
            material.diffuse_color = (int(.5 * pt.x + 1000) + int(.5 * pt.z)) & 1 ? vec3{.3, .3, .3} : vec3{.3, .2, .1};
        }
    }

    for (const Sphere &s : spheres) { // Intersect with all spheres
        auto [intersection, d] = ray_sphere_intersect(orig, dir, s);
        if (!intersection || d > nearest_dist) continue;
        nearest_dist = d;
        pt = orig + dir * nearest_dist;
        N = (pt - s.center).normalized();
        material = s.material;
    }
    return {nearest_dist < 1000, pt, N, material};
}

// Function to reflect a ray
vec3 reflect(const vec3 &I, const vec3 &N) {
    return I - N * 2.f * (I * N);
}

// Function to refract a ray using Snell's law
vec3 refract(const vec3 &I, const vec3 &N, const float eta_t, const float eta_i = 1.f) {
    float cosi = -std::max(-1.f, std::min(1.f, I * N));
    if (cosi < 0) return refract(I, -N, eta_i, eta_t);
    float eta = eta_i / eta_t;
    float k = 1 - eta * eta * (1 - cosi * cosi);
    return k < 0 ? vec3{1, 0, 0} : I * eta + N * (eta * cosi - std::sqrt(k));
}

// Recursive ray tracing function
vec3 cast_ray(const vec3 &orig, const vec3 &dir, int depth) {
    auto [hit, point, N, material] = scene_intersect(orig, dir);
    if (depth > 4 || !hit)
        return {0.2, 0.7, 0.8}; // Background color

    vec3 reflect_dir = reflect(dir, N).normalized();
    vec3 refract_dir = refract(dir, N, material.refractive_index).normalized();
    vec3 reflect_color = cast_ray(point, reflect_dir, depth + 1);
    vec3 refract_color = cast_ray(point, refract_dir, depth + 1);

    float diffuse_light_intensity = 0, specular_light_intensity = 0;
    for (const vec3 &light : lights) { // Compute lighting
        vec3 light_dir = (light - point).normalized();
        auto [hit, shadow_pt, trash_nrm, trash_mat] = scene_intersect(point, light_dir);
        if (hit && (shadow_pt - point).norm() < (light - point).norm()) continue;
        diffuse_light_intensity += std::max(0.f, light_dir * N);
        specular_light_intensity += std::pow(std::max(0.f, -reflect(-light_dir, N) * dir), material.specular_exponent);
    }

    return material.diffuse_color * diffuse_light_intensity * material.albedo[0] +
           vec3{1., 1., 1.} * specular_light_intensity * material.albedo[1] +
           reflect_color * material.albedo[2] +
           refract_color * material.albedo[3];
}
