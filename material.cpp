//
// Created by randan on 4/7/17.
//

#include "material.h"

material::material() {}

material::material(const sf::Color& color, double alpha, double reflect, double refract)
        : color(color), alpha(alpha), reflect(reflect), refract(refract) {}

material::material(const material& obj)
    : color(obj.color)
    , alpha(obj.alpha)
    , reflect(obj.reflect)
    , refract(obj.refract)
{}
