//
// Created by randan on 4/4/17.
//

#include "vector3d.h"

vector3d::vector3d(const vector3d& obj)
        : x_(obj.x_)
          , y_(obj.y_)
          , z_(obj.z_) {}

vector3d::vector3d(const long double& x, const long double& y, const long double& z)
        : x_(x)
          , y_(y)
          , z_(z) {}

vector3d vector3d::operator+(const vector3d& obj) const {
    return vector3d(x_ + obj.x_, y_ + obj.y_, z_ + obj.z_);
}

vector3d vector3d::operator-(const vector3d& obj) const {
    return vector3d(x_ - obj.x_, y_ - obj.y_, z_ - obj.z_);
}

vector3d vector3d::operator*(long double x) const {
    return vector3d(x_ * x, y_ * x, z_ * x);
}

vector3d operator*(long double x, const vector3d& vec){
    return vec * x;
}

vector3d vector3d::operator/(long double x) const {
    return vector3d(x_ / x, y_ / x, z_ / x);
}

vector3d& vector3d::operator*=(const long double& x) {
    (*this) = (*this) * x;
    return *this;
}

vector3d& vector3d::operator/=(const long double& x) {
    (*this) = (*this) / x;
    return *this;
}

vector3d vector3d::operator-() const {
    return vector3d(-x_, -y_, -z_);
}

long double scalar_product(const vector3d& vec1, const vector3d& vec2) {
    return vec1.x_ * vec2.x_ + vec1.y_ * vec2.y_ + vec1.z_ * vec2.z_;
}

vector3d vector3d::normalize() const {
    long double module = this->module();
    return vector3d(x_ / module, y_ / module, z_ / module);
}

vector3d cross_product(const vector3d& vec1, const vector3d& vec2) {
    return vector3d(vec1.y_ * vec2.z_ - vec1.z_ * vec2.y_,
                    vec1.z_ * vec2.x_ - vec1.x_ * vec2.z_,
                    vec1.x_ * vec2.y_ - vec1.y_ * vec2.x_);
}

long double cos(const vector3d& vec1, const vector3d& vec2) {
    return scalar_product(vec1, vec2) / vec1.module() / vec2.module();
}

long double vector3d::module() const {
    return std::sqrt(scalar_product(*this, *this));
}

vector3d::vector3d() {}
