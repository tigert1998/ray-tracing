#include "parallelogram.h"

#include "triangle.h"

using glm::vec3;
using std::make_shared, std::array, std::pair, std::shared_ptr;
using std::optional;

Parallelogram::Parallelogram(array<vec3, 3> p,
                             shared_ptr<Material> material_ptr) {
  for (int i = 0; i <= 1; i++) p_[i] = p[i];
  p_[2] = p[1] + p[2] - p[0];
  p_[3] = p[2];
  material_ptr_ = material_ptr;
}

optional<HitRecord> Parallelogram::Hit(const Ray &ray,
                                       pair<double, double> t_range) const {
  vec3 n = normalize(cross(p_[2] - p_[0], p_[1] - p_[2]));
  double den = dot(ray.direction(), n);
  double num = dot(p_[0] - ray.position(), n);
  double t = num / den;
  if (std::isnan(t) || std::isinf(t)) return std::nullopt;
  if (t < t_range.first || t > t_range.second) return std::nullopt;
  vec3 hit_point = ray.position() + (float)t * ray.direction();
  {
    array<double, 4> dot_values;
    for (int i = 0; i < 4; i++) {
      vec3 a = p_[i], b = p_[(i + 1) % 4];
      dot_values[i] = dot(cross(a - hit_point, b - hit_point), n);
    }
    for (int i = 0; i < 4; i++)
      if (dot_values[i] * dot_values[(i + 1) % 4] < 0) return std::nullopt;
  }
  HitRecord record;
  record.t = t;
  record.normal = dot(n, ray.direction()) < 0 ? n : -n;
  record.material_ptr = material_ptr_.get();
  return record;
}