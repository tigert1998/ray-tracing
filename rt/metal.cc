#include "metal.h"

#include <glm/geometric.hpp>

using std::optional;
using std::pair;
using namespace glm;

Metal::Metal(vec3 albedo) : albedo_(albedo) {}

optional<pair<vec3, Ray>> Metal::Scatter(const Ray &ray,
                                         const HitRecord &record) const {
  if (dot(ray.direction(), record.normal) >= 0) return std::nullopt;
  vec3 p = ray.position() + float(record.t) * ray.direction();
  return pair<vec3, Ray>(albedo_,
                         Ray(p, reflect(ray.direction(), record.normal)));
}
