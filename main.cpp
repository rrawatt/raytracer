#include "scene.h"
#include <vector>
#include <fstream>
#include <iostream>

int main() {
    constexpr int width = 1920;
    constexpr int height = 1080;
    constexpr float fov = 1.05;

    std::vector<vec3> framebuffer(width * height);

#pragma omp parallel for
    for (int pix = 0; pix < width * height; pix++) {
        float dir_x = (pix % width + 0.5) - width / 2.;
        float dir_y = -(pix / width + 0.5) + height / 2.;
        float dir_z = -height / (2. * tan(fov / 2.));
        framebuffer[pix] = cast_ray(vec3{0, 0, 0}, vec3{dir_x, dir_y, dir_z}.normalized());
    }

    std::ofstream ofs("./out.ppm", std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (vec3& color : framebuffer) {
        float max = std::max(1.f, std::max(color[0], std::max(color[1], color[2])));
        for (int chan : {0, 1, 2})
            ofs << (char)(255 * color[chan] / max);
    }

    std::cout << "Image rendered successfully to 'out.ppm'.\n";
    return 0;
}
