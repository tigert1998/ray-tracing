#include "sphere.h"

using namespace glm;
using boost::optional;
using boost::none;

Sphere::Sphere(vec3 position, double radius, std::shared_ptr<Material> material_ptr): radius_(radius), position_(position), material_ptr_(material_ptr) { }

optional<HitRecord> Sphere::Hit(const Ray &ray, std::pair<double, double> t_range) const {
    double a = pow(length(ray.direction()), 2);
    double b = 2 * dot(ray.direction(), ray.position() - this->position());
    double c = pow(length(ray.position() - this->position()), 2) - pow(this->radius(), 2);
    double discriminant = pow(b, 2) - 4 * a * c;
    if (discriminant < 0) {
        return none;
    }
    double t = (-b - pow(discriminant, 0.5)) / (2 * a);
    if (t_range.first <= t && t <= t_range.second) {
        HitRecord record;
        record.t = t;
        vec3 p = ray.position() + float(t) * ray.direction();
        record.normal = normalize(p - this->position());
        record.material_ptr = material_ptr();
        return record;
    }
    t = (-b + pow(discriminant, 0.5)) / (2 * a);
    if (t_range.first <= t && t <= t_range.second) {
        HitRecord record;
        record.t = t;
        vec3 p = ray.position() + float(t) * ray.direction();
        record.normal = normalize(p - this->position());
        record.material_ptr = material_ptr();
        return record;
    }
    return none;
}

double Sphere::radius() const {
    return radius_;
}

vec3 Sphere::position() const {
    return position_;
}

std::weak_ptr<Material> Sphere::material_ptr() const {
    return material_ptr_;
}
