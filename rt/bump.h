#pragma once

#include <optional>

#include "material.h"
#include "perlin_noise.h"

class Bump : public Material {
 public:
  Bump() = delete;
  Bump(glm::vec3 albedo);
  std::optional<std::pair<glm::vec3, Ray>> Scatter(
      const Ray &ray, const HitRecord &record) const;

 private:
  static const PerlinNoise generator_;
  glm::vec3 albedo_;
};
