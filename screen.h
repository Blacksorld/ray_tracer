//
// Created by randan on 4/4/17.
//

#pragma once


#include "vector3d.h"

class screen {
public:
    screen(const vector3d& origin_, const vector3d& dir1_, const vector3d& dir2_, const long double& length1_,
           const long double& length2_);

    void set_resolution(int axis1, int axis2);

    friend class ray_tracer;
private:
    vector3d origin_;
    vector3d dir1_;
    vector3d dir2_;
    long double length1_;
    long double length2_;
};

