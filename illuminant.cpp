//
// Created by randan on 4/5/17.
//

#include "illuminant.h"
#include <iostream>

illuminant::illuminant(const vector3d& origin_, const long double& intensity_)
        : origin_(origin_)
          , intensity_(intensity_) {}

long double
illuminant::get_point_intensity(const vector3d& point, const vector3d& normal,
                                const std::vector<object*>& objects) const {
    vector3d direction = point - origin_;
    long double dist2 = scalar_product(direction, direction);
    long double cosinus = cos(-direction, normal);
    if(cosinus < 0)
        return 0;
    long double result = intensity_ * cosinus / dist2;

    ray illuminant_ray(origin_, direction);
    long double point_dist = (point - origin_).module();
    for(auto object : objects) {
        vector3d intersection_point;
        bool is_intersection = object->intersect(illuminant_ray, &intersection_point);
        if(is_intersection && (intersection_point - origin_).module() + 1e-9 < point_dist)
            return 0;
    }

    return result;
}
