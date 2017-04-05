//
// Created by randan on 4/4/17.
//

#include "screen.h"

screen::screen(const vector3d& origin_, const vector3d& dir1_, const vector3d& dir2_, const long double& length1_,
               const long double& length2_)
        : origin_(origin_)
          , dir1_(dir1_)
          , dir2_(dir2_)
          , length1_(length1_)
          , length2_(length2_) {}

void screen::set_resolution(int axis1, int axis2) {
    dir1_ *= (length1_ / axis1 / dir1_.module());
    dir2_ *= (length2_ / axis2 / dir2_.module());
}
