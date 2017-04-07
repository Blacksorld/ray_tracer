//
// Created by randan on 4/4/17.
//

#include "triangle.h"



bool triangle::intersect(const ray& ray, vector3d* const point) const {
    vector3d e1 = v1_ - v0_;
    vector3d e2 = v2_ - v0_;
    vector3d P = cross_product(ray.direction_, e2);
    double det = scalar_product(e1, P);

    if(std::abs(det) < 1e-9)
        return false;

    vector3d T = ray.origin_ - v0_;

    double u = scalar_product(T, P) / det;
    if(u < 0 || u > 1)
        return false;

    vector3d Q = cross_product(T, e1);
    double v = scalar_product(ray.direction_, Q) / det;

    if(v < 0 || u + v > 1)
        return false;

    double t = scalar_product(e2, Q) / det;

    if (t < 1e-9)
        return false;
    *point = ray.origin_ + t * ray.direction_;
    return true;
}

vector3d triangle::normal(const vector3d& point) const {
    return normal_;
}

triangle::triangle(const sf::Color& color, double alpha, const double& reflectivity, double refractivity, const vector3d& v0_,
                   const vector3d& v1_, const vector3d& v2_)
        : object(color, alpha, reflectivity, refractivity)
          , v0_(v0_)
          , v1_(v1_)
          , v2_(v2_)
          , normal_(cross_product(v1_ - v0_, v2_ - v0_).normalize()) {}
