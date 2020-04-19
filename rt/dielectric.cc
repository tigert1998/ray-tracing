#include "dielectric.h"

#include <glm/geometric.hpp>
#include <iostream>

using std::optional;
using std::pair, std::cerr, std::endl;
using namespace glm;

Dielectric::Dielectric(vec3 attenuation, double refractive_index) {
  attenuation_ = attenuation;
  refractive_index_ = refractive_index;
}

optional<pair<vec3, Ray>> Dielectric::Scatter(const Ray &ray,
                                              const HitRecord &record) const {
  static auto VectorIsValid = [](vec3 v) -> bool {
    if (v[0] == 0 && v[1] == 0 && v[2] == 0) return false;
    for (int i = 0; i < 3; i++)
      if (isnan(v[i])) return false;
    return true;
  };
  if (dot(ray.direction(), record.normal) >= 0) {
    // from inner to outer
    auto p = ray.position() + float(record.t) * ray.direction();
    auto d = refract(ray.direction(), -record.normal,
                     (float)refractive_index_ / 1.0f);
    if (!VectorIsValid(d)) return std::nullopt;
    return pair<vec3, Ray>(attenuation_, Ray(p, d));
  } else {
    // from outer to inner
    auto p = ray.position() + float(record.t) * ray.direction();
    auto d = refract(ray.direction(), record.normal,
                     1.0f / (float)refractive_index_);
    if (!VectorIsValid(d)) return std::nullopt;
    return pair<vec3, Ray>(attenuation_, Ray(p, d));
  }
}
