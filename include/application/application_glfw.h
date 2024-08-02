#pragma once

#include "application.h"

// Forward declarations
struct GLFWwindow;

namespace MandelbrotSet {

class ApplicationGLFW : public Application {
 public:
  ApplicationGLFW(const Size& window_size);
  ~ApplicationGLFW() override;
  bool ShouldClose() const override;
  void SwapBuffers() override;
  void PollEvents() override;
  Point GetCursorPosition() const override;
  double_t GetTime() const override;

 private:
  GLFWwindow* window_;
};

}  // namespace MandelbrotSet