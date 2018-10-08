#include <glm/gtc/random.hpp>

#include "bump.h"

using boost::optional, boost::none;
using std::pair;
using glm::vec3, glm::reflect, glm::ballRand;

const PerlinNoise Bump::generator_ = PerlinNoise(256);

Bump::Bump(vec3 albedo): albedo_(albedo) { }

optional<pair<vec3, Ray>> Bump::Scatter(const Ray &ray, const HitRecord &record) const {
    auto p = ray.position() + float(record.t) * ray.direction();
    auto normal_disturbance = generator_.DerivativeNoise(p * 12.0f) * 0.05;
    auto n = normalize(record.normal + vec3(normal_disturbance));
    if (dot(ray.direction(), n) > 0)
        return none;
    auto d = reflect(ray.direction(), n) + ballRand(0.1f);
    return pair<vec3, Ray>(albedo_, Ray(p, d));
}

