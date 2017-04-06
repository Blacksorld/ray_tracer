//
// Created by randan on 4/4/17.
//

#pragma once

#include <cmath>

class vector3d {
public:
    vector3d();

    vector3d(const vector3d &obj);

    vector3d(const long double &x, const long double &y, const long double &z);

    vector3d operator+(const vector3d &obj) const;

    vector3d operator-(const vector3d &obj) const;

    vector3d operator*(long double x) const;

    friend vector3d operator*(long double x, const vector3d& vec);

    vector3d operator/(long double x) const;

    vector3d& operator*=(const long double& x);

    vector3d& operator/=(const long double& x);

    vector3d operator-() const;

    vector3d normalize() const;

    friend long double scalar_product(const vector3d &vec1, const vector3d &vec2);

    friend vector3d cross_product(const vector3d &vec1, const vector3d &vec2);

    friend long double cos(const vector3d &vec1, const vector3d &vec2);

    long double module() const;

    vector3d projection(const vector3d& vec) const;

private:
    long double x_;
    long double y_;
    long double z_;
};