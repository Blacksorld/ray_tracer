//
// Created by randan on 4/4/17.
//

#pragma once


#include "object.h"

class triangle : public object{
public:
    triangle(const sf::Color& color, const long double& reflectivity, const vector3d& v0_, const vector3d& v1_,
             const vector3d& v2_);

    bool intersect(const ray& ray, vector3d* const point) const override;

    vector3d normal(const vector3d& point) const override;

private:
    vector3d v0_;
    vector3d v1_;
    vector3d v2_;
};


