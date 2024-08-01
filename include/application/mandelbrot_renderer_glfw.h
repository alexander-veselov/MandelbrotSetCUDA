#pragma once

#include "mandelbrot_renderer.h"
#include "image.h"

namespace MandelbrotSet {

// TODO: Split MandelbrotRendererGLFW class
// At the moment it's responsible for too many things:
// 1. Mandelbrot set calculation
// 2. Storing image data
// 3. Rendering using specific GLFW implementation
class MandelbrotRendererGLFW : public MandelbrotRenderer {
 public:
  MandelbrotRendererGLFW(uint32_t width, uint32_t height);
  void Render(const Complex& center, double_t zoom,
              const RenderOptions& render_options = {}) override;

 private:
  Image image_;
  Complex center_;
  double_t zoom_;
};

}  // namespace MandelbrotSet