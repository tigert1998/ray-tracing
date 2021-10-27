#include "bump.h"

#include "glm/gtc/random.hpp"

using glm::vec3, glm::reflect, glm::ballRand;
using std::pair;

const PerlinNoise Bump::generator_ = PerlinNoise(256);

Bump::Bump(vec3 albedo) : albedo_(albedo) {}

std::optional<pair<vec3, Ray>> Bump::Scatter(const Ray &ray,
                                             const HitRecord &record) const {
  auto p = ray.position() + float(record.t) * ray.direction();
  auto normal_disturbance = generator_.DerivativeNoise(p * 12.0f) * 0.05;
  auto n = normalize(record.normal + vec3(normal_disturbance));
  if (dot(ray.direction(), n) > 0) return std::nullopt;
  auto d = reflect(ray.direction(), n) + ballRand(0.1f);
  return pair<vec3, Ray>(albedo_, Ray(p, d));
}
