#include "mandelbrot/application/application_glfw.h"

#include "mandelbrot/application/config.h"
#include "mandelbrot/application/mandelbrot_renderer_glfw.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

namespace mandelbrot {

static MouseButton ConvertMouseButton(int32_t button) {
  switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
      return MouseButton::kLeft;
    case GLFW_MOUSE_BUTTON_RIGHT:
      return MouseButton::kRight;
    default:
      return MouseButton::kOther;
  }
}

static MouseAction ConvertMouseAction(int32_t action) {
  switch (action) {
    case GLFW_PRESS:
      return MouseAction::kPress;
    case GLFW_RELEASE:
      return MouseAction::kRelease;
    default:
      return MouseAction::kOther;
  }
}

static ScrollAction ConvertScrollAction(double_t y_offset) {
  if (y_offset > 0.) {
    return ScrollAction::kScrollUp;
  } else if (y_offset < 0.) {
    return ScrollAction::kScrollDown;
  } else {
    return ScrollAction::kOther;
  }
}

static KeyButton ConvertKeyButton(int32_t button) {
  switch (button) {
    case GLFW_KEY_ESCAPE:
      return KeyButton::kEscape;
    case GLFW_KEY_PRINT_SCREEN:
      return KeyButton::kPrintScreen;
    case GLFW_KEY_LEFT:
      return KeyButton::kLeft;
    case GLFW_KEY_RIGHT:
      return KeyButton::kRight;
    case GLFW_KEY_UP:
      return KeyButton::kUp;
    case GLFW_KEY_DOWN:
      return KeyButton::kDown;
    case GLFW_KEY_COMMA:
      return KeyButton::kComma;
    case GLFW_KEY_PERIOD:
      return KeyButton::kPeriod;
    case GLFW_KEY_M:
      return KeyButton::kM;
    case GLFW_KEY_P:
      return KeyButton::kP;
    default:
      return KeyButton::kOther;
  }
}

static KeyAction ConvertKeyAction(int32_t action) {
  static_assert(std::is_same_v<MouseAction, KeyAction>,
                "Implement ConvertKeyAction properly in case if KeyAction and "
                "MouseAction are not the same");
  return ConvertMouseAction(action);
}

ApplicationGLFW::ApplicationGLFW(const Size& window_size)
    : Application{window_size,
                  std::make_unique<MandelbrotRendererGLFW>(window_size)} {

  if (glfwInit() != GLFW_TRUE) {
    glfwTerminate();
  }

  auto monitor = static_cast<GLFWmonitor*>(nullptr);
  switch (GetConfig().window_mode) {
    case WindowMode::kWindowed:
      break;
    case WindowMode::kFullscreen:
      monitor = glfwGetPrimaryMonitor();
      break;
    case WindowMode::kBorderless:
      glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
      break;
  }

  constexpr auto kWinwowName = "Mandelbrot Set";
  window_ = glfwCreateWindow(window_size_.width, window_size_.height,
                             kWinwowName, monitor, nullptr);

  glfwSetWindowUserPointer(window_, this);
  glfwMakeContextCurrent(window_);
  glfwSwapInterval(GetConfig().enable_vsync);

  if (GetConfig().window_mode == WindowMode::kWindowed) {
    glfwSetWindowAttrib(window_, GLFW_RESIZABLE, GLFW_FALSE);
  }

  glViewport(0, 0, window_size_.width, window_size_.height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0.0, window_size_.width, 0.0, window_size_.height, 0.0, 1.0);

  auto MouseButtonCallback = [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
    const auto app_button = ConvertMouseButton(button);
    const auto app_action = ConvertMouseAction(action);
    const auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    application->MouseButtonCallback(app_button, app_action);
  };

  auto CursorPosCallback = [](GLFWwindow* window, double_t x_pos, double_t y_pos) {
    const auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    application->CursorPositionCallback(Point{x_pos, y_pos});
  };

  auto ScrollCallback = [](GLFWwindow* window, double_t x_offset, double_t y_offset) {
    const auto app_action = ConvertScrollAction(y_offset);
    const auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    application->ScrollCallback(app_action);
  };

  auto KeyCallback = [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
    const auto app_button = ConvertKeyButton(key);
    const auto app_action = ConvertKeyAction(action);
    const auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
    application->KeyCallback(app_button, app_action);
  };

  glfwSetMouseButtonCallback(window_, MouseButtonCallback);
  glfwSetCursorPosCallback(window_, CursorPosCallback);
  glfwSetScrollCallback(window_, ScrollCallback);
  glfwSetKeyCallback(window_, KeyCallback);
}

ApplicationGLFW::~ApplicationGLFW() {
  glfwDestroyWindow(window_);
  glfwTerminate();
}

void ApplicationGLFW::Close() {
  glfwSetWindowShouldClose(window_, GL_TRUE);
}

bool ApplicationGLFW::ShouldClose() const {
  return glfwWindowShouldClose(window_);
}

void ApplicationGLFW::SwapBuffers() {
  glfwSwapBuffers(window_);
}

void ApplicationGLFW::PollEvents() {
  glfwPollEvents();
}

Point ApplicationGLFW::GetCursorPosition() const {
  auto x = double_t{};
  auto y = double_t{};
  glfwGetCursorPos(window_, &x, &y);
  return Point{x, y};
}

double_t ApplicationGLFW::GetTime() const {
  return glfwGetTime();
}

}  // namespace mandelbrot