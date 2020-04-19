#include "hitable_list.h"

using std::optional;
using std::pair;

std::vector<std::shared_ptr<Hitable>> &HitableList::list() { return list_; }

optional<HitRecord> HitableList::Hit(const Ray &ray,
                                     pair<double, double> t_range) const {
  optional<HitRecord> final_record = std::nullopt;
  for (const auto obj_ptr : list_) {
    auto record = obj_ptr->Hit(ray, t_range);
    if (record == std::nullopt) continue;
    if (final_record == std::nullopt) {
      final_record = record;
    } else {
      if (final_record->t > record->t) {
        final_record = record;
      }
    }
  }
  return final_record;
}
