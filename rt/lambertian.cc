#include "lambertian.h"

#include "glm/gtc/random.hpp"
#include "textures/constant_texture.h"

using std::optional;
using std::pair, std::function, std::shared_ptr, std::make_shared;
using namespace glm;

Lambertian::Lambertian(function<double()> dice,
                       shared_ptr<Texture> texture_ptr) {
  dice_ = dice;
  texture_ptr_ = texture_ptr;
}

Lambertian::Lambertian(std::function<double()> dice, glm::vec3 color) {
  dice_ = dice;
  texture_ptr_ = make_shared<ConstantTexture>(color);
}

optional<pair<vec3, Ray>> Lambertian::Scatter(const Ray &ray,
                                              const HitRecord &record) const {
  if (dot(ray.direction(), record.normal) >= 0) return std::nullopt;
  auto p = ray.position() + float(record.t) * ray.direction();
  vec3 target;
  {
    vec3 next_dir = sphericalRand(1.f);
    auto dot_value = dot(next_dir, record.normal);
    if (dot_value < 0) {
      next_dir += record.normal * std::abs(2 * dot_value);
    }
    target = p + next_dir;
  }
  auto albedo = texture_ptr_->Value(0, 0, p);
  return pair<vec3, Ray>(albedo, Ray(p, target - p));
}
