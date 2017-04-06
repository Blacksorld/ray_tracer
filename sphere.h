//
// Created by randan on 4/4/17.
//

#pragma once


#include "object.h"

class sphere : public object {
public:
    sphere(const sf::Color& color, const double& reflectivity, const vector3d& center_, double radius_);

    bool intersect(const ray& ray, vector3d* const point) const override;

    vector3d normal(const vector3d& point) const override;

private:
    vector3d center_;
    double radius_;
};


