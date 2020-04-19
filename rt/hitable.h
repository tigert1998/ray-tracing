#pragma once

struct HitRecord;

#include <memory>
#include <optional>

#include "material.h"
#include "ray.h"

struct HitRecord {
  double t;
  glm::vec3 normal;
  std::weak_ptr<Material> material_ptr;
};

class Hitable {
 public:
  virtual std::optional<HitRecord> Hit(
      const Ray &ray, std::pair<double, double> t_range) const = 0;
};
