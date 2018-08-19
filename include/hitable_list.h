#pragma once

#include <memory>
#include <vector>

#include "hitable.h"

class HitableList: public Hitable {
public:
    boost::optional<HitRecord> Hit(const Ray &ray, std::pair<double, double> t_range) const;
    std::vector<std::shared_ptr<Hitable>> &list();

private:
    std::vector<std::shared_ptr<Hitable>> list_;
};
