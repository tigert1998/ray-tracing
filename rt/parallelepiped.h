#pragma once

#include <array>
#include <functional>

#include "hitable_list.h"

//    1
//   /|\
//  ? 0 ?
//  |X X|
//  2 ? 3
//   \|/
//    ?

class Parallelepiped : public Hitable {
 private:
  HitableList list_;
  void AddCorner(const std::array<glm::vec3, 4> &p,
                 std::shared_ptr<Material> material_ptr);

 public:
  Parallelepiped() = delete;
  Parallelepiped(std::array<glm::vec3, 4> p,
                 std::shared_ptr<Material> material_ptr);
  Parallelepiped(glm::vec3 lengths, std::shared_ptr<Material> material_ptr,
                 std::function<glm::vec3(glm::vec3)> transform);
  std::optional<HitRecord> Hit(const Ray &ray,
                               std::pair<double, double> t_range) const;
};