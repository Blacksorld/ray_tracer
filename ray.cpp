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
