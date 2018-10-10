#include "parallelogram.h"
#include "triangle.h"

using std::make_shared, std::array, std::pair, std::shared_ptr;
using boost::optional;
using glm::vec3;

Parallelogram::Parallelogram(array<vec3, 3> p, shared_ptr<Material> material_ptr) {
	list_.list().push_back(make_shared<Triangle>(p, material_ptr));
	p[0] = p[2] + p[1] - p[0];
	list_.list().push_back(make_shared<Triangle>(p, material_ptr));
}

optional<HitRecord> Parallelogram::Hit(const Ray &ray, pair<double, double> t_range) const {
	return list_.Hit(ray, t_range);
}