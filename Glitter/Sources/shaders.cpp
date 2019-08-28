#include "shaders.hpp"

#include <iostream>

char buffer[512];
int success;

unsigned int Shaders::shader(const char* pathname, GLenum type) {
  std::string source_str = utils.slurp(pathname);
  const char* source = source_str.c_str();
  unsigned int id = glCreateShader(type);

  glShaderSource(id, 1, &source, NULL);
  glCompileShader(id);

  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(id, 512, NULL, buffer);
    fprintf(stderr, "Shader info log:\n%s\n", buffer);
    assert(false);
  }

  return id;
}

void Shaders::prepare()
{
  unsigned int shaderProgram, vertex, fragment;

  for(int i = 0; i < shaders.size(); ++i) {
    shaderProgram = glCreateProgram();
    vertex = shader(shaders[i].vert, GL_VERTEX_SHADER);
    fragment = shader(shaders[i].frag, GL_FRAGMENT_SHADER);

    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);

    shaders[i].id = shaderProgram;

    glDeleteShader(vertex);
    glDeleteShader(fragment);
  }
}

unsigned int Shaders::use(int i) {
  unsigned int id = shaders[i].id;
  glUseProgram(id);

  return id;
}
