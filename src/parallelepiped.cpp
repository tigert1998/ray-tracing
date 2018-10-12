#include "parallelepiped.h"
#include "parallelogram.h"

using std::array, std::shared_ptr, std::pair, std::make_shared, std::function;
using boost::optional;
using glm::vec3;

Parallelepiped::Parallelepiped(array<vec3, 4> p, shared_ptr<Material> material_ptr) {
	auto fourth = [] (vec3 a, vec3 b, vec3 c) -> vec3 {
		return c + b - a;
	};
	array<vec3, 4> q;
	q[3] = fourth(p[0], p[1], p[2]);
	q[2] = fourth(p[0], p[1], p[3]);
	q[1] = fourth(p[0], p[2], p[3]);
	q[0] = fourth(p[1], q[2], q[3]);
	AddCorner(p, material_ptr);
	AddCorner(q, material_ptr);
}

optional<HitRecord> Parallelepiped::Hit(const Ray &ray, pair<double, double> t_range) const {
	return list_.Hit(ray, t_range);
}

void Parallelepiped::AddCorner(const array<vec3, 4> &p, shared_ptr<Material> material_ptr) {
	for (int i = 1; i <= 3; i++) {
		int x = i, y = i + 1 == 4 ? 1 : x + 1;
		list_.list().push_back(
			make_shared<Parallelogram>(array<vec3, 3>({p[0], p[x], p[y]}), material_ptr)
		);
	}
};

Parallelepiped::Parallelepiped(
	vec3 lengths,
	shared_ptr<Material> material_ptr,
	function<vec3(vec3)> transform
) {
	array<vec3, 4> p, q;
	p[0] = vec3(0);
	for (int i = 1; i <= 3; i++) {
		p[i] = vec3(0);
		p[i][i - 1] = lengths[i - 1];
	}
	q[0] = lengths;
	for (int i = 1; i <= 3; i++) {
		q[i] = lengths;
		q[i][i - 1] = 0;
	}
	for (int i = 0; i < 4; i++) {
		p[i] = transform(p[i]);
		q[i] = transform(q[i]);
	}
	AddCorner(p, material_ptr);
	AddCorner(q, material_ptr);
}