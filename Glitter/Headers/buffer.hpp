#ifndef BUFFER
#define BUFFER
#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "glitter.hpp"
#include "textures.hpp"

struct Buffer {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;

  unsigned int n, vao, vbo, ebo;

  void setup();
  void bind();
  void update_vertices();
  void update_indices();
  void unbind();
  void draw();

  void gen_plane();
  void set_plane_tex_coords(int offset, Textures::Mapping &m);
};

#endif
