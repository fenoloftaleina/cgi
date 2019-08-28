#ifndef UTILS
#define UTILS
#pragma once

#include <string>
#include <cstdio>
#include <cerrno>

struct Utils {
  std::string slurp(const char *filename);
  float randf();
};

#endif
