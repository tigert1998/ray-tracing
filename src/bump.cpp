#include <glm/gtc/random.hpp>

#include "bump.h"

using boost::optional;
using boost::none;
using std::pair;
using glm::vec3;
using glm::reflect;
using glm::ballRand;

const PerlinNoise Bump::generator_ = PerlinNoise(256);

Bump::Bump(vec3 albedo): albedo_(albedo) { }

optional<pair<vec3, Ray>> Bump::Scatter(const Ray &ray, const HitRecord &record) const {
    if (dot(ray.direction(), record.normal) >= 0)
        return none;
    auto p = ray.position() + float(record.t) * ray.direction();
    auto n = normalize(record.normal + vec3(generator_.DerivativeNoise(p * 100.f) * 0.2));
    auto d = reflect(ray.direction(), n) + ballRand(0.1f);
    return pair<vec3, Ray>(albedo_, Ray(p, d));
}

