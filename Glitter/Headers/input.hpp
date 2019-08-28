#ifndef INPUT
#define INPUT
#pragma once

#include <GLFW/glfw3.h>

#include "glitter.hpp"

struct Input {
  glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

  float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
  float pitch =  0.0f;
  float lastX =  800.0f / 2.0;
  float lastY =  600.0 / 2.0;
  float fov   =  45.0f;

  bool firstMouse = true;

  float xoffset;
  float yoffset;
  glm::vec3 front;
  float mouse_sensitivity = 0.1f;

  void mouse_callback(GLFWwindow*, double xpos, double ypos);
  void process_input(GLFWwindow *window, float dt);
};

#endif
