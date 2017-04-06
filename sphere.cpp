//
// Created by randan on 4/4/17.
//

#include "sphere.h"



bool sphere::intersect(const ray& ray, vector3d* const point) const {
    double b = 2 * scalar_product(ray.origin_ - center_, ray.direction_);
    double c = radius_ * radius_ - scalar_product(ray.origin_ - center_, ray.origin_ - center_);
    double discr = (b * b + 4 * c);

    if(discr < 0)
        return false;

    if(discr == 0){
        double t = -b / 2;
        if(t < 1e-9)
            return false;
        *point = ray.direction_ * t;
    }else{
        double t1 = (std::sqrt(discr) - b) / 2;
        double t2 = (-std::sqrt(discr) - b) / 2;
        if(t1 < 1e-9 && t2 < 1e-9)
            return false;
        if(std::min(t1, t2) < 1e-9)
            *point = ray.origin_ + ray.direction_ * std::max(t1, t2);
        else
            *point = ray.origin_ + ray.direction_ * std::min(t1, t2);
    }

    return true;
}

vector3d sphere::normal(const vector3d& point) const {
    return (point - center_).normalize();
}

sphere::sphere(const sf::Color& color, const double& reflectivity, const vector3d& center_, double radius_)
        : object(color, reflectivity), center_(center_), radius_(radius_) {}
