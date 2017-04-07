//
// Created by randan on 4/7/17.
//

#pragma once

#include <string>
#include "SFML/Graphics.hpp"

class material {
public:
    material();

    material(const sf::Color& color, double alpha, double reflect, double refract);

    material(const material& obj);

    sf::Color color = sf::Color();
    double alpha = 0;
    double reflect = 0;
    double refract = 0;
};


