#include "constant_texture.h"

using glm::vec3;

ConstantTexture::ConstantTexture(glm::vec3 color) { color_ = color; }

glm::vec3 ConstantTexture::Value(double u, double v, const glm::vec3 &p) const {
  return color_;
}