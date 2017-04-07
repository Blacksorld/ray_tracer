//
// Created by randan on 4/4/17.
//

#pragma once

#include <cmath>

class vector3d {
public:
    vector3d();

    vector3d(const vector3d &obj);

    vector3d(const double &x, const double &y, const double &z);

    vector3d operator+(const vector3d &obj) const;

    vector3d operator-(const vector3d &obj) const;

    vector3d operator*(double x) const;

    friend vector3d operator*(double x, const vector3d& vec);

    vector3d operator/(double x) const;

    vector3d& operator*=(const double& x);

    vector3d& operator/=(const double& x);

    vector3d operator-() const;

    vector3d normalize() const;

    friend double scalar_product(const vector3d &vec1, const vector3d &vec2);

    friend vector3d cross_product(const vector3d &vec1, const vector3d &vec2);

    friend double cos(const vector3d &vec1, const vector3d &vec2);

    double module() const;

    vector3d projection(const vector3d& vec) const;



    friend class object;
    friend class quadrilateral;
    friend class triangle;
    friend class sphere;
private:
    double x_;
    double y_;
    double z_;
};