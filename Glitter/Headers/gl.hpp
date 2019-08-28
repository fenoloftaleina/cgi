#ifndef GL
#define GL
#pragma once

#include <GLFW/glfw3.h>
#include "glitter.hpp"

struct Gl {
  const int width = 1280;
  const int height = 800;
  GLFWwindow* window;

  void basic_init(void (*mouse_callback)(GLFWwindow*, double, double));
};

#endif
