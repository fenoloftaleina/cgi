#ifndef TEXTURE
#define TEXTURE
#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "glitter.hpp"

struct Textures {
  struct Mapping {
    unsigned int texture_id;
    std::string name;
    float x1, y1, x2, y2;
    unsigned char* texture_data;
  };

  const int texture_size = 1024;

  std::vector<std::string> resources_pathnames = {
    "../resources/zero.png",
    "../resources/one.png"
  };
  std::vector<Mapping> mappings;

  void prepare();
};

#endif
