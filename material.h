#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

struct Material {
    float refractive_index = 1;
    float albedo[4] = {2,0,0,0};
    vec3 diffuse_color = {0,0,0};
    float specular_exponent = 0;
};
extern const Material metal;
extern const Material glass;
extern const Material trans;
extern const Material mirror;

#endif