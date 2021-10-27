#pragma once

#include "glm/glm.hpp"
#include "texture.h"

class ConstantTexture : public Texture {
 private:
  glm::vec3 color_;

 public:
  ConstantTexture() = delete;
  ConstantTexture(glm::vec3 color);
  glm::vec3 Value(double u, double v, const glm::vec3 &p) const;
};