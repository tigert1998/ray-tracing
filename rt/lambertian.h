#pragma once

#include <functional>
#include <memory>

#include "material.h"
#include "textures/texture"

class Lambertian : public Material {
 public:
  Lambertian(std::function<double()> dice, glm::vec3 color);
  Lambertian(std::function<double()> dice,
             std::shared_ptr<Texture> texture_ptr);
  std::optional<std::pair<glm::vec3, Ray>> Scatter(
      const Ray &ray, const HitRecord &record) const;

 private:
  std::function<double()> dice_;
  std::shared_ptr<Texture> texture_ptr_;
};
