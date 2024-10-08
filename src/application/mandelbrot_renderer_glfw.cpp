#include "mandelbrot/application/mandelbrot_renderer_glfw.h"

#include "mandelbrot/core/cuda/mandelbrot_set.h"

#include <GLFW/glfw3.h>

namespace mandelbrot {

MandelbrotRendererGLFW::MandelbrotRendererGLFW(const Size& size)
    : MandelbrotRenderer{size} {}

bool MandelbrotRendererGLFW::IsDirty(
    const Complex& center, double_t zoom,
    const RenderOptions& render_options) const {
  return center != center_ || zoom != zoom_ || render_options != render_options_;
}

void MandelbrotRendererGLFW::RenderImage(const Image& image) const {
  static_assert(std::is_same_v<Image::Type, uint32_t>,
                "Image::Type must be uint32_t");
  glDrawPixels(image.GetWidth(), image.GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE,
               image.GetData());
}

}  // namespace mandelbrot