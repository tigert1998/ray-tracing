#pragma once

#include <array>

#include "hitable_list.h"

class Parallelogram : public Hitable {
 private:
  std::array<glm::vec3, 4> p_;
  std::shared_ptr<Material> material_ptr_;

 public:
  Parallelogram() = delete;
  Parallelogram(std::array<glm::vec3, 3> p,
                std::shared_ptr<Material> material_ptr);
  std::optional<HitRecord> Hit(const Ray &ray,
                               std::pair<double, double> t_range) const;
};