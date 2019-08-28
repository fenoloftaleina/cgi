#ifndef SHADERS
#define SHADERS
#pragma once

#include <string>
#include <vector>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "utils.hpp"

struct Shaders
{
  struct Shader
  {
    const char* vert;
    const char* frag;
    unsigned int id;
  };

  Utils utils;

  std::vector<Shader> shaders = {
    {
      .vert = "../resources/shaders/first.vert",
      .frag = "../resources/shaders/first.frag"
    }
  };

  unsigned int shader(const char* pathname, GLenum type);
  void prepare();
  unsigned int use(int i);
};

#endif
