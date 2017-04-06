//
// Created by randan on 4/5/17.
//

#pragma once


#include "vector3d.h"
#include "object.h"
#include <vector>

class illuminant {
public:
    illuminant(const vector3d& origin_, const double& intensity_);

    double get_point_intensity(const vector3d& point, const vector3d& normal,
                                    const std::vector<object*>& objects) const;
private:
    vector3d origin_;
    double intensity_;
};


