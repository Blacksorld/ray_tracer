//
// Created by randan on 4/4/17.
//

#include "sphere.h"

sphere::sphere(const sf::Color& color, const vector3d& center, const long double radius)
        : object(color)
          , center_(center)
          , radius_(radius) {}

bool sphere::intersect(const ray& ray, vector3d* const point) const {
    long double b = 2 * scalar_product(ray.origin_ - center_, ray.direction_);
    long double c = radius_ * radius_ - scalar_product(ray.origin_ - center_, ray.origin_ - center_);
    long double discr = (b * b + 4 * c);

    if(discr < 0)
        return false;

    if(discr == 0){
        long double t = -b / 2;
        if(t < 0)
            return false;
        *point = ray.direction_ * t;
    }else{
        long double t1 = (std::sqrt(discr) - b) / 2;
        long double t2 = (-std::sqrt(discr) - b) / 2;
        if(t1 < 0 && t2 < 0)
            return false;
        if(std::min(t1, t2) < 0)
            *point = ray.origin_ + ray.direction_ * std::max(t1, t2);
        else
            *point = ray.origin_ + ray.direction_ * std::min(t1, t2);
    }

    return true;
}

vector3d sphere::normal(const vector3d& point) const {
    return (point - center_).normalize();
}
