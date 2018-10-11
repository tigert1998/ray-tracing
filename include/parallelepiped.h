#pragma once

#include "hitable_list.h"

#include <array>

//    1
//   /|\
//  ? 0 ?
//  |X X|
//  2 ? 3
//   \|/
//    ?

class Parallelepiped: public Hitable {
private:
	HitableList list_;

public:
	Parallelepiped() = delete;
	Parallelepiped(std::array<glm::vec3, 4> p, std::shared_ptr<Material> material_ptr);
	boost::optional<HitRecord> Hit(const Ray &ray, std::pair<double, double> t_range) const;
};