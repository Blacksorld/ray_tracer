//
// Created by randan on 4/7/17.
//

#include "rt_parser.h"

rt_parser::rt_parser(const std::string& file_name, unsigned int axis1_resolution,
                     unsigned int axis2_resolution, unsigned int antialiasing_coefficient)
        : input_(file_name)
          ,axis1_resolution_(axis1_resolution)
          , axis2_resolution_(axis2_resolution)
          , antialiasing_coefficient_(
        antialiasing_coefficient) {}

void rt_parser::parse() {
    parse_viewport_();
    parse_materials_();
    parse_lights_();
    parse_geometry_();
    input_.close();
}

void rt_parser::render() {
    ray_tracer tracer(objects_, illuminants_);
    tracer.render(screen_, observer_, axis1_resolution_, axis2_resolution_, antialiasing_coefficient_);
    tracer.draw();
}

void rt_parser::parse_viewport_() {
    std::string s;
    bool section_started = false;
    double x = 0, y = 0, z = 0;
    double x1 = 0, y1 = 0, z1 = 0;
    double x2 = 0, y2 = 0, z2 = 0;
    double x3 = 0, y3 = 0, z3 = 0;

    while(true){
        input_ >> s;

        if(!section_started){
            if(s == "viewport")
                section_started = true;
            else
                continue;
        }

        if(s == "endviewport") {
            observer_ = vector3d(x, y, z);
            build_screen_(vector3d(x1, y1, z1), vector3d(x2, y2, z2), vector3d(x3, y3, z3));
            break;
        }

        if(s == "origin"){
            input_ >> x >> y >> z;
            continue;
        }

        if(s == "topleft"){
            input_ >> x1 >> y1 >> z1;
            continue;
        }

        if(s == "bottomleft"){
            input_ >> x2 >> y2 >> z2;
            continue;
        }

        if(s == "topright"){
            input_ >> x3 >> y3 >> z3;
            continue;
        }
    }
}

void rt_parser::parse_materials_() {
    std::string s;
    bool section_started = false;
    std::string name = "";
    unsigned int r = 0, g = 0 , b = 0;
    double alpha = 0, reflect = 0, refract = 0;

    while(true){
        input_ >> s;

        if(!section_started){
            if(s == "materials")
                section_started = true;
            else
                continue;
        }

        if(s == "endmaterials")
            break;

        if(s == "entry") {
            while(true) {
                input_ >> s;

                if(s == "endentry") {
                    materials_[name] = material(sf::Color((sf::Uint8)r, (sf::Uint8)g, (sf::Uint8)b),
                                                alpha, reflect, refract);
                    break;
                }

                if(s == "name"){
                    input_ >> name;
                    continue;
                }

                if(s == "color") {
                    input_ >> r >> g >> b;
                    continue;
                }

                if(s == "alpha"){
                    input_ >> alpha;
                    continue;
                }

                if(s == "reflect") {
                    input_ >> reflect;
                    continue;
                }

                if(s == "refract") {
                    input_ >> refract;
                    continue;
                }
            }
        }
    }
}

void rt_parser::parse_lights_() {
    std::string s;
    bool section_started = false;

    double e_power = 0, e_dist = 0;
    double k = 1;
    double x = 0, y = 0, z = 0;
    double power = 0;


    while(true) {
        input_ >> s;

        if(!section_started){
            if(s == "lights")
                section_started = true;
            else
                continue;
        }

        if(s == "endlights") {
            break;
        }

        if(s == "reference"){
            while(true) {
                input_ >> s;

                if(s == "endreference") {
                    k = e_dist * e_dist / e_power;
                    break;
                }

                if(s == "power"){
                    input_ >> e_power;
                    continue;
                }

                if(s == "distance")
                    input_ >> e_dist;
            }
        }

        if(s == "point") {
            while(true){
                input_ >> s;

                if(s == "endpoint"){
                    illuminants_.emplace_back(vector3d(x, y, z), k * power);
                    break;
                }

                if(s == "coords"){
                    input_ >> x >> y >> z;
                    continue;
                }

                if(s == "power"){
                    input_ >> power;
                    continue;
                }
            }
        }
    }
}

void rt_parser::parse_geometry_() {
    std::string s;
    bool section_started = false;

    while(true) {
        input_ >> s;

        if(!section_started){
            if(s == "geometry")
                section_started = true;
            else
                continue;
        }

        if(s == "endgeometry")
            break;

        if(s == "sphere")
            parse_sphere_();

        if(s == "triangle")
            parse_triangle_();

        if(s == "quadrangle")
            parse_quadrangle_();
    }
}

void rt_parser::build_screen_(const vector3d& topleft, const vector3d& bottomleft, const vector3d topright) {
    vector3d d1 = topright - topleft;
    vector3d d2 = bottomleft - topleft;
    screen_ = screen(topleft, d1, d2, d1.module(), d2.module());
}

void rt_parser::parse_sphere_() {
    std::string s;
    double x = 0, y = 0, z = 0;
    double radius = 0;
    std::string material_id;
    while(true){
        input_ >> s;

        if(s == "endsphere") {
            material mat = materials_[material_id];
            objects_.push_back(new sphere(mat.color, mat.alpha, mat.reflect, mat.refract,
                                          vector3d(x, y, z), radius));
            break;
        }

        if(s == "coords"){
            input_ >> x >> y >> z;
            continue;
        }

        if(s == "radius"){
            input_ >> radius;
            continue;
        }

        if(s == "material"){
            input_ >> material_id;
            continue;
        }
    }
}

void rt_parser::parse_triangle_() {
    std::string s;
    double x[3], y[3], z[3];
    unsigned int cur_vertex = 0;
    std::string material_id;

    while(true){
        input_ >> s;

        if(s == "endtriangle") {
            material mat = materials_[material_id];
            objects_.push_back(
                    new triangle(mat.color, mat.alpha, mat.reflect, mat.refract,
                                 vector3d(x[0], y[0], z[0]), vector3d(x[1], y[1], z[1]),
                                 vector3d(x[2], y[2], z[2])));
            break;
        }

        if(s == "vertex"){
            input_ >> x[cur_vertex] >> y[cur_vertex] >> z[cur_vertex];
            ++cur_vertex;
            continue;
        }

        if(s == "material"){
            input_ >> material_id;
            continue;
        }
    }
}

void rt_parser::parse_quadrangle_() {
    std::string s;
    double x[4], y[4], z[4];
    unsigned int cur_vertex = 0;
    std::string material_id;

    while(true){
        input_ >> s;

        if(s == "endquadrangle") {
            material mat = materials_[material_id];
            objects_.push_back(new quadrilateral(mat.color, mat.alpha, mat.reflect, mat.refract,
                                                 vector3d(x[0], y[0], z[0]),
                                                 vector3d(x[1], y[1], z[1]),
                                                 vector3d(x[2], y[2], z[2]),
                                                 vector3d(x[3], y[3], z[3])));
            break;
        }

        if(s == "vertex"){
            input_ >> x[cur_vertex] >> y[cur_vertex] >> z[cur_vertex];
            ++cur_vertex;
            continue;
        }

        if(s == "material"){
            input_ >> material_id;
            continue;
        }
    }
}

rt_parser::~rt_parser() {
    for(auto object : objects_)
        delete object;
}
