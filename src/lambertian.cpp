#include <glm/gtc/random.hpp>

#include "lambertian.h"

using boost::optional, boost::none;
using std::pair, std::function;
using namespace glm;

Lambertian::Lambertian(function<double()> dice, vec3 albedo) {
    dice_ = dice;
    albedo_ = albedo;
}

optional<pair<vec3, Ray>> Lambertian::Scatter(const Ray &ray, const HitRecord &record) const {
    if (dot(ray.direction(), record.normal) >= 0)
        return none;
    auto p = ray.position() + float(record.t) * ray.direction();
    auto target = p + record.normal + ballRand(0.9f);
    return pair<vec3, Ray>(albedo_, Ray(p, target - p));
}
