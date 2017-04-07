//
// Created by randan on 4/7/17.
//

#pragma once

#include "ray_tracer.h"
#include "triangle.h"
#include "sphere.h"
#include "quadrilateral.h"
#include "material.h"
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <exception>


class rt_parser {
public:
    rt_parser(const std::string& file_name, unsigned int axis1_resolution = 800,
              unsigned int axis2_resolution = 800, unsigned int antialiasing_coefficient = 4);

    virtual ~rt_parser();

    void parse();

    void render();

private:
    std::ifstream input_;
    unsigned int axis1_resolution_;
    unsigned int axis2_resolution_;
    unsigned int antialiasing_coefficient_;

    vector3d observer_;
    screen screen_;
    std::map<std::string, material> materials_;
    std::vector<illuminant> illuminants_;
    std::vector<object*> objects_;

    void parse_viewport_();

    void parse_materials_();

    void parse_lights_();

    void parse_geometry_();

    void build_screen_(const vector3d& topleft,
                       const vector3d& bottomleft, const vector3d topright);

    void parse_sphere_();

    void parse_triangle_();

    void parse_quadrangle_();
};


