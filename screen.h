//
// Created by randan on 4/4/17.
//

#pragma once


#include "vector3d.h"

class screen {
public:
    screen();

    screen(const vector3d& origin_, const vector3d& dir1_, const vector3d& dir2_, const double& length1_,
           const double& length2_);

    screen(const screen& obj);

    void set_resolution(int axis1, int axis2);

    friend class ray_tracer;
private:
    vector3d origin_;
    vector3d dir1_;
    vector3d dir2_;
    double length1_;
    double length2_;
};


