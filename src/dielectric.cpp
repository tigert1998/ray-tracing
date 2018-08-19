#include <iostream>

#include <boost/format.hpp>

#include <glm/geometric.hpp>

#include "dielectric.h"

using boost::optional;
using boost::none;
using boost::format;
using std::pair;
using std::cerr;
using std::endl;
using namespace glm;

Dielectric::Dielectric(vec3 attenuation, double refractive_index) {
    attenuation_ = attenuation;
    refractive_index_ = refractive_index;
}

optional<pair<vec3, Ray>> Dielectric::Scatter(const Ray &ray, const HitRecord &record) const {
    static auto VectorIsValid = [] (vec3 v) -> bool {
        for (int i = 0; i < 3; i++) if (isnan(v[i])) return false;
        return true;
    };
    if (dot(ray.direction(), record.normal) >= 0) {
        // from inner to outer
        auto p = ray.position() + float(record.t) * ray.direction();
        auto d = refract(ray.direction(), -record.normal, (float) refractive_index_ / 1.0f);
        if (!VectorIsValid(d))
            return none;
        return pair<vec3, Ray>(attenuation_, Ray(p, d));
    } else {
        // from outer to inner
        auto p = ray.position() + float(record.t) * ray.direction();
        auto d = refract(ray.direction(), record.normal, 1.0f / (float) refractive_index_);
        if (!VectorIsValid(d))
            return none;
        return pair<vec3, Ray>(attenuation_, Ray(p, d));
    }
}

