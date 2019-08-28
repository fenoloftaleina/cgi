#include "buffer.hpp"

#include <iostream>

void Buffer::update_vertices()
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
}

void Buffer::update_indices()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

void Buffer::setup()
{
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  bind();

  update_vertices();
  update_indices();

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}


void Buffer::bind()
{
  glBindVertexArray(vao);
}


void Buffer::unbind()
{
  glBindVertexArray(0);
}


void Buffer::draw()
{
  // glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
  glDrawElementsInstanced(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0, 13);
  // glDrawArrays(GL_TRIANGLES, 0, n);
}


void Buffer::gen_plane()
{
  vertices.reserve(32);
  vertices = {
    // -0.5f, -0.5f, -0.5f, 0.1f, 0.2f, 0.3f,
    // -0.5f,  0.5f, -0.5f, 0.1f, 0.5f, 0.7f,
    // 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    // 0.5f, -0.5f, -0.5f,  0.4f, 0.0f, 0.5f,

    // -0.5f, -0.5f,  0.5f, 0.1f, 0.2f, 0.3f, 0.0f, 0.0f,
    // 0.5f, -0.5f,  0.5f,  0.4f, 0.0f, 0.5f, 1.0f, 0.0f,
    // 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
    // -0.5f,  0.5f,  0.5f, 0.1f, 0.5f, 0.7f, 0.0f, 1.0f
    -0.5f, -0.5f,  0.5f, 0.1f, 0.2f, 0.3f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.4f, 0.0f, 0.5f, 0.0159570, 0.0f,
    0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f, 0.0159570, 0.0168359,
    -0.5f,  0.5f,  0.5f, 0.1f, 0.5f, 0.7f, 0.0f, 0.0168359

    // -0.5f,  0.5f,  0.5f, 0.1f, 0.2f, 0.3f,
    // -0.5f,  0.5f, -0.5f, 0.4f, 0.0f, 0.5f,
    // -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
    // -0.5f, -0.5f,  0.5f, 0.1f, 0.5f, 0.7f,
    //
    // 0.5f,  0.5f,  0.5f,  0.1f, 0.2f, 0.3f,
    // 0.5f, -0.5f,  0.5f,  0.1f, 0.5f, 0.7f,
    // 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
    // 0.5f,  0.5f, -0.5f,  0.4f, 0.0f, 0.5f,
    //
    // -0.5f, -0.5f, -0.5f, 0.1f, 0.2f, 0.3f,
    // 0.5f, -0.5f, -0.5f,  0.4f, 0.0f, 0.5f,
    // 0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    // -0.5f, -0.5f,  0.5f, 0.1f, 0.5f, 0.7f,
    //
    // -0.5f,  0.5f, -0.5f, 0.1f, 0.2f, 0.3f,
    // -0.5f,  0.5f,  0.5f, 0.1f, 0.5f, 0.7f,
    // 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
    // 0.5f,  0.5f, -0.5f,  0.4f, 0.0f, 0.5f
  };

  n = 36;
  indices.resize(n);

  for(int i = 0; i < n / 6; ++i) {
    indices[i * 6] = i * 4;
    indices[i * 6 + 1] = i * 4 + 1;
    indices[i * 6 + 2] = i * 4 + 2;
    indices[i * 6 + 3] = i * 4 + 2;
    indices[i * 6 + 4] = i * 4 + 3;
    indices[i * 6 + 5] = i * 4;
  }
}

void Buffer::set_plane_tex_coords(int offset, Textures::Mapping &m)
{
  vertices[offset + 6] = m.x1;
  vertices[offset + 7] = m.y1;
  vertices[offset + 14] = m.x2;
  vertices[offset + 15] = m.y1;
  vertices[offset + 22] = m.x2;
  vertices[offset + 23] = m.y2;
  vertices[offset + 30] = m.x1;
  vertices[offset + 31] = m.y2;
}
