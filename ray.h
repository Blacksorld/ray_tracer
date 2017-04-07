//
// Created by randan on 4/4/17.
//

#pragma once

#include "vector3d.h"

class ray {
public:
    ray(const vector3d& origin, const vector3d& direction);

    ray(const ray& obj);

    ray mirror(const vector3d& point, const vector3d& normal) const;

    friend class ray_tracer;
    friend class object;
    friend class sphere;
    friend class triangle;
    friend class quadrilateral;
private:
    vector3d origin_;
    vector3d direction_; // |direction_| = 1
};
