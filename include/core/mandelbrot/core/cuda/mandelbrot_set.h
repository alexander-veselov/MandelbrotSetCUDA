#pragma once

#include "mandelbrot/core/cuda/defines.h"

namespace mandelbrot {
namespace cuda {

void Visualize(uint32_t* image, uint32_t image_width, uint32_t image_height,
               double_t center_real = -0.5, double_t center_imag = 0.0,
               double_t zoom_factor = 1.0, uint32_t max_iterations = 256u,
               uint32_t coloring_mode = 0u, uint32_t palette = 0u,
               bool smoothing = false);

}
}  // namespace mandelbrot