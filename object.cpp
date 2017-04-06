//
// Created by randan on 4/4/17.
//

#include "object.h"

const sf::Color& object::get_color() const {
    return color_;
}

object::object(const sf::Color& color, const long double& reflectivity)
        : color_(color)
        , reflectivity_(reflectivity / 100)
{}

object::~object() {

}

long double object::getReflectivity() const {
    return reflectivity_;
}