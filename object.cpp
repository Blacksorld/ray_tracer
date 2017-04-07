//
// Created by randan on 4/4/17.
//

#include "object.h"

const sf::Color& object::get_color() const {
    return color_;
}


object::~object() {

}

double object::getReflectivity() const {
    return reflectivity_;
}

object::object(const sf::Color& color_, double alpha_, double reflectivity_, double refractiviry_)
        : color_(color_), alpha_(alpha_), reflectivity_(reflectivity_), refractiviry_(refractiviry_) {}

double object::getAlpha() const {
    return alpha_;
}

double object::getRefractiviry() const {
    return refractiviry_;
}
