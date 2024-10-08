#include "mandelbrot/application/screenshot_renderer.h"

#include "mandelbrot/core/utils.h"
#include "mandelbrot/core/cuda/mandelbrot_set.h"

#include "mandelbrot/application/config.h"

#include <sstream>
#include <iomanip>
#include <string>

namespace mandelbrot {

std::string ToString(double_t value) {
  auto stream = std::ostringstream{};
  stream.precision(std::numeric_limits<double_t>::max_digits10);
  stream << value;
  auto string = stream.str();
  if (string.empty()) {
    throw std::runtime_error{"Failed to convert double to string"};
  }
  // Replace minus with 'm' symbol to be more readable as filename format
  if (string[0] == '-') {
    string[0] = 'm';
  }
  return string;
}

std::filesystem::path NameScreenshot(const Complex& center, double_t zoom) {
  constexpr auto kExtension = ".png";
  constexpr auto kSeparator = "_";
  auto filename = ToString(center.real);
  filename += kSeparator;
  filename += ToString(center.imag);
  filename += kSeparator;
  filename += ToString(zoom);
  filename += kExtension;
  const auto screenshots_path = std::filesystem::path{GetConfig().screenshots_folder};
  if (!std::filesystem::exists(screenshots_path.parent_path())) {
    std::filesystem::create_directory(screenshots_path.parent_path());
  }
  return screenshots_path / filename;
}

ScreenshotRenderer::ScreenshotRenderer(const Size& size)
    : MandelbrotRenderer{size} {}

bool ScreenshotRenderer::IsDirty(const Complex& center, double_t zoom,
                                 const RenderOptions& render_options) const {
  return true;
}

void ScreenshotRenderer::RenderImage(const Image& image) const {
  WriteImage(image, NameScreenshot(center_, zoom_));
}

}