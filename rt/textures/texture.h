#pragma once

#include "glm/glm.hpp"

class Texture {
 public:
  virtual glm::vec3 Value(double u, double v, const glm::vec3 &p) const = 0;
};