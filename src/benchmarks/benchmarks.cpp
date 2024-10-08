#include <benchmark/benchmark.h>

#include "mandelbrot/core/coloring_mode.h"
#include "mandelbrot/core/complex.h"
#include "mandelbrot/core/cuda/mandelbrot_set.h"
#include "mandelbrot/core/image.h"
#include "mandelbrot/core/palette.h"

namespace mandelbrot {

static void Default(benchmark::State& state) {
  constexpr auto kSize = Size{1024, 768};
  constexpr auto kCenter = Complex{-0.5, 0.0};
  constexpr auto kZoom = 1.0;
  constexpr auto kMaxIterations = 1024;
  constexpr auto kColoringMode = static_cast<uint32_t>(ColoringMode::kMode1);
  constexpr auto kPalette = static_cast<uint32_t>(Palette::kBluePalette);
  constexpr auto kSmoothing = true;

  auto image = Image{kSize};
  for (auto _ : state) {
    cuda::Visualize(image.GetData(), image.GetWidth(), image.GetHeight(),
                    kCenter.real, kCenter.imag, kZoom, kMaxIterations,
                    kColoringMode, kPalette, kSmoothing);
  }
}
BENCHMARK(Default)
  ->MinTime(1)
  ->Unit(benchmark::kMillisecond);

static void DifferentResolution(benchmark::State& state) {
  const auto width = static_cast<uint32_t>(state.range(0));
  const auto height = static_cast<uint32_t>(state.range(1));
  const auto size = Size{width, height};
  constexpr auto kCenter = Complex{ -0.5, 0.0 };
  constexpr auto kZoom = 1.0;
  constexpr auto kMaxIterations = 1024;
  constexpr auto kColoringMode = static_cast<uint32_t>(ColoringMode::kMode1);
  constexpr auto kPalette = static_cast<uint32_t>(Palette::kBluePalette);
  constexpr auto kSmoothing = true;

  auto image = Image{size};
  for (auto _ : state) {
    cuda::Visualize(image.GetData(), image.GetWidth(), image.GetHeight(),
                    kCenter.real, kCenter.imag, kZoom, kMaxIterations,
                    kColoringMode, kPalette, kSmoothing);
  }
}
BENCHMARK(DifferentResolution)
  ->Args({ 640,  360  }) // nHD
  ->Args({ 854,  480  }) // FWVGA
  ->Args({ 960,  540  }) // qHD
  ->Args({ 1024, 576  }) // WSVGA
  ->Args({ 1280, 720  }) // HD
  ->Args({ 1366, 768  }) // FWXGA
  ->Args({ 1600, 900  }) // HD+
  ->Args({ 1920, 1080 }) // Full HD
  ->Args({ 2560, 1440 }) // QHD
  ->Args({ 3200, 1800 }) // QHD+
  ->Args({ 3840, 2160 }) // 4K UHD
  ->Args({ 5120, 2880 }) // 5K
  ->Args({ 7680, 4320 }) // 8K UHD
  ->MinTime(1)
  ->Unit(benchmark::kMillisecond);

static void DifferentMaxIterations(benchmark::State& state) {
  constexpr auto kSize = Size{1024, 768};
  constexpr auto kCenter = Complex{ -0.5, 0.0 };
  constexpr auto kZoom = 1.0;
  const auto max_iterations = static_cast<int32_t>(state.range(0));
  constexpr auto kColoringMode = static_cast<uint32_t>(ColoringMode::kMode1);
  constexpr auto kPalette = static_cast<uint32_t>(Palette::kBluePalette);
  constexpr auto kSmoothing = true;

  auto image = Image{kSize};
  for (auto _ : state) {
    cuda::Visualize(image.GetData(), image.GetWidth(), image.GetHeight(),
                    kCenter.real, kCenter.imag, kZoom, max_iterations,
                    kColoringMode, kPalette, kSmoothing);
  }
}
BENCHMARK(DifferentMaxIterations)
  ->Arg(128)
  ->Arg(256)
  ->Arg(512)
  ->Arg(1024)
  ->Arg(2048)
  ->Arg(4096)
  ->Arg(8192)
  ->MinTime(1)
  ->Unit(benchmark::kMillisecond);

}