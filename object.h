//
// Created by randan on 4/4/17.
//

#pragma once

#include "ray.h"
#include "vector3d.h"
#include <SFML/Graphics.hpp>

class object {
public:
    object(const sf::Color& color, const long double& reflectivity);

    virtual ~object();

    virtual bool intersect(const ray &ray, vector3d* const point) const = 0;

    virtual vector3d normal(const vector3d &point) const = 0;

    const sf::Color& get_color() const;

    long double getReflectivity() const;

protected:
    sf::Color color_;
    long double reflectivity_;
};


