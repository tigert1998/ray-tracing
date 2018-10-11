#include "parallelepiped.h"
#include "parallelogram.h"

using std::array, std::shared_ptr, std::pair, std::make_shared;
using boost::optional;
using glm::vec3;

Parallelepiped::Parallelepiped(array<vec3, 4> p, shared_ptr<Material> material_ptr) {
	auto add_corner = [&] (const array<vec3, 4> &p) {
		for (int i = 1; i <= 3; i++) {
			int x = i, y = i + 1 == 4 ? 1 : x + 1;
			list_.list().push_back(
				make_shared<Parallelogram>(array<vec3, 3>({p[0], p[x], p[y]}), material_ptr)
			);
		}
	};
	auto fourth = [] (vec3 a, vec3 b, vec3 c) -> vec3 {
		return c + b - a;
	};
	array<vec3, 4> q;
	q[3] = fourth(p[0], p[1], p[2]);
	q[2] = fourth(p[0], p[1], p[3]);
	q[1] = fourth(p[0], p[2], p[3]);
	q[0] = fourth(p[1], q[2], q[3]);
	add_corner(p);
	add_corner(q);
}

optional<HitRecord> Parallelepiped::Hit(const Ray &ray, pair<double, double> t_range) const {
	return list_.Hit(ray, t_range);
}
