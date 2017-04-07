//
// Created by randan on 4/4/17.
//

#pragma once

#include "ray.h"
#include "vector3d.h"
#include <SFML/Graphics.hpp>
#include <array>

class object {
public:
    object(const sf::Color& color, const double& reflectivity);

    virtual ~object();

    virtual bool intersect(const ray& ray, vector3d* const point) const = 0;

    virtual vector3d normal(const vector3d& point) const = 0;

    const sf::Color& get_color() const;

    double getReflectivity() const;

protected:
    sf::Color color_;
    double reflectivity_;
};


