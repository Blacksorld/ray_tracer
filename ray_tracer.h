//
// Created by randan on 4/5/17.
//

#pragma once


#include "screen.h"
#include "object.h"
#include "illuminant.h"
#include <vector>

class ray_tracer {
public:
    ray_tracer(const screen& screen, const vector3d& observer, const std::vector<object*>& objects,
                   const std::vector<illuminant>& illuminants, const unsigned int& axis1_resolution,
                   const unsigned int& axis2_resolution);

    void render();

    void draw() const;

private:
    screen screen_;
    vector3d observer_;
    std::vector<object*> objects_;
    std::vector<illuminant> illuminants_;
    unsigned int axis1_resolution_;
    unsigned int axis2_resolution_;
    sf::VertexArray real_screen_;

    sf::Color get_screen_color_(const vector3d& screen_point) const;
    sf::Color get_point_color_(const vector3d& point, const object* point_object, const vector3d& dir) const;
};


