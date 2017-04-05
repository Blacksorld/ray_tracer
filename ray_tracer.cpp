//
// Created by randan on 4/5/17.
//

#include "ray_tracer.h"

ray_tracer::ray_tracer(const screen& screen, const vector3d& observer, const std::vector<object*>& objects,
                       const std::vector<illuminant>& illuminants, const unsigned int& axis1_resolution,
                       const unsigned int& axis2_resolution)
        : screen_(screen)
          , observer_(observer)
          , objects_(objects)
          , illuminants_(illuminants)
          , axis1_resolution_(axis1_resolution)
          , axis2_resolution_(axis2_resolution)
          , real_screen_(sf::Points, axis1_resolution * axis2_resolution) {
    for(std::size_t  i = 0; i < axis1_resolution; ++i)
        for(std::size_t j = 0; j < axis2_resolution; ++j)
            real_screen_[i * axis2_resolution + j].position = sf::Vector2f(i, j);
    screen_.set_resolution(axis1_resolution, axis2_resolution);
}

void ray_tracer::render() {
    for(unsigned int step1 = 0; step1 < axis1_resolution_; ++step1)
        for(unsigned int step2 = 0; step2 < axis2_resolution_; ++step2)
            real_screen_[step1 * axis2_resolution_ + step2].color =
                    get_screen_color_(screen_.origin_ + step1 * screen_.dir1_ + step2 * screen_.dir2_);
}

void ray_tracer::draw() const {
    sf::RenderWindow window(sf::VideoMode(axis1_resolution_, axis2_resolution_), "ray_tracer");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(real_screen_);
        window.display();
    }
}

sf::Color ray_tracer::get_screen_color_(const vector3d& screen_point) const {
    ray screen_ray(screen_point, screen_point - observer_);
    long double min_distance = 1e18;
    sf::Color result_color;

    for(auto object : objects_) {
        vector3d intersection_point;
        bool is_intersection = object->intersect(screen_ray, &intersection_point);
        if(!is_intersection)
            continue;

        long double current_distance = (intersection_point - screen_point).module();
        if(current_distance < min_distance){
            min_distance = current_distance;
            result_color = get_point_color_(intersection_point, object, observer_ - screen_point);
        }
    }

    return result_color;
}

sf::Color ray_tracer::get_point_color_(const vector3d& point, const object* point_object, const vector3d& dir) const {
    long double intensity = 0.1;
    vector3d normal = point_object->normal(point);
    if(scalar_product(normal, dir) < 0)
        normal = -normal;

    for(const auto& illuminant : illuminants_)
        intensity += illuminant.get_point_intensity(point, normal, objects_);

    sf::Color result = point_object->get_color();
    result.r = (sf::Uint8)std::min((int)(std::round(intensity * (long double)(result.r))), 255);
    result.g = (sf::Uint8)std::min((int)(std::round(intensity * (long double)(result.g))), 255);
    result.b = (sf::Uint8)std::min((int)(std::round(intensity * (long double)(result.b))), 255);

    return result;
}
