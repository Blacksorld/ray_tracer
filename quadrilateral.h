//
// Created by randan on 4/5/17.
//

#pragma once


#include "object.h"
#include "triangle.h"

class quadrilateral : public object{
public:
    quadrilateral(const sf::Color& color, const double& reflectivity, const vector3d& v00_, const vector3d& v01_,
                  const vector3d& v11_, const vector3d& v10_);

    bool intersect(const ray& ray, vector3d* const point) const override;

    vector3d normal(const vector3d& point) const override;

private:
    vector3d v00_;
    vector3d v01_;
    vector3d v11_;
    vector3d v10_;
    vector3d normal_;
};


