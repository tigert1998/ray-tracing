#include "hitable_list.h"

using boost::optional;
using boost::none;
using std::pair;

std::vector<std::shared_ptr<Hitable>> &HitableList::list() {
    return list_;
}

optional<HitRecord> HitableList::Hit(const Ray &ray, pair<double, double> t_range) const {
    optional<HitRecord> final_record = none;
    for (const auto obj_ptr: list_) {
        auto record = obj_ptr->Hit(ray, t_range);
        if (record == none) continue;
        if (final_record == none) {
            final_record = record;
        } else {
            if (final_record->t > record->t) {
                final_record = record;
            }
        }
    }
    return final_record;
}
