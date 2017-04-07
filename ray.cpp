//
// Created by randan on 4/4/17.
//

#include "ray.h"

ray::ray(const vector3d& start, const vector3d& direction)
        : origin_(start)
          , direction_(direction.normalize()) {}

ray::ray(const ray& obj)
        : origin_(obj.origin_)
          , direction_(obj.direction_) {}

ray ray::mirror(const vector3d& point, const vector3d& normal) const {
    return ray(point, direction_ - 2 * scalar_product(direction_, normal) * normal);
}
