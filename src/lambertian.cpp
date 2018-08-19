#include "lambertian.h"

using boost::optional;
using boost::none;
using std::pair;
using std::function;
using namespace glm;

vec3 Lambertian::RandomInUnitSphere() const {
    vec3 result;
    do {
        for (int i = 0; i < 3; i++) result[i] = dice_();
    } while (length(result) >= 1);
    return result;
}

Lambertian::Lambertian(function<double()> dice, vec3 albedo) {
    dice_ = dice;
    albedo_ = albedo;
}

optional<pair<vec3, Ray>> Lambertian::Scatter(const Ray &ray, const HitRecord &record) const {
    if (dot(ray.direction(), record.normal) >= 0)
        return none;
    auto p = ray.position() + float(record.t) * ray.direction();
    auto target = p + record.normal + RandomInUnitSphere();
    return pair<vec3, Ray>(albedo_, Ray(p, target - p));
}
