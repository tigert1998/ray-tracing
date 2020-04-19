#pragma once

#include <utility>

#include "hitable.h"
#include "material.h"

class Sphere : public Hitable {
 public:
  Sphere() = delete;
  Sphere(glm::vec3 position, double radius,
         std::shared_ptr<Material> material_ptr);
  std::optional<HitRecord> Hit(const Ray& ray,
                               std::pair<double, double> t_range) const;
  double radius() const;
  glm::vec3 position() const;
  Material* material_ptr() const;

 private:
  double radius_;
  glm::vec3 position_;
  std::shared_ptr<Material> material_ptr_;
};
