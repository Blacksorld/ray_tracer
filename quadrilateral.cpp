//
// Created by randan on 4/5/17.
//

#include "quadrilateral.h"


bool quadrilateral::intersect(const ray& ray, vector3d* const point) const {
    vector3d E01 = v10_ - v00_;
    vector3d E03 = v01_ - v00_;
    vector3d P1 = cross_product(ray.direction_, E03);
    double det1 = scalar_product(E01, P1);

    if(std::abs(det1) < 1e-9)
        return false;

    vector3d T1 = ray.origin_ - v00_;
    double a1 = scalar_product(T1, P1) / det1;

    if(a1 < 0 || a1 > 1)
        return false;

    vector3d Q1 = cross_product(T1, E01);
    double b1 = scalar_product(ray.direction_, Q1) / det1;

    if(b1 < 0 || b1 > 1)
        return false;

    if(a1 + b1 > 1) {
        vector3d E23 = v01_ - v11_;
        vector3d E21 = v10_ - v11_;
        vector3d P2 = cross_product(ray.direction_, E21);
        double det2 = scalar_product(E23, P2);

        if(std::abs(det2) < 1e-9)
            return false;

        vector3d T2 = ray.origin_ - v11_;
        double a2 = scalar_product(T2, P2) / det2;

        if(a2 < 0)
            return false;

        vector3d Q2 = cross_product(T2, E23);
        double b2 = scalar_product(ray.direction_, Q2) / det2;

        if(b2 < 0)
            return false;
    }

    double t = scalar_product(E03, Q1) / det1;

    if(t < 1e-9)
        return false;

    *point = ray.origin_ + t * ray.direction_;
    return true;
}

vector3d quadrilateral::normal(const vector3d& point) const {
    return normal_;
}

quadrilateral::quadrilateral(const sf::Color& color, const double& reflectivity, const vector3d& v00_,
                             const vector3d& v01_, const vector3d& v11_, const vector3d& v10_)
        : object(color, reflectivity)
          , v00_(v00_)
          , v01_(v01_)
          , v11_(v11_)
          , v10_(v10_)
          , normal_(cross_product(v01_ - v00_, v11_ - v00_).normalize()) {}


