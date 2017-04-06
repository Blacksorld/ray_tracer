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

object* ray_tracer::tracey_(class ray ray, vector3d* const point) const {
    long double min_distance = 1e18;
    object* intersection_object = nullptr;

    for(auto object : objects_) {
        vector3d intersection_point;
        if(object->intersect(ray, &intersection_point)){
            long double current_distance = (intersection_point - ray.origin_).module();
            if(current_distance < min_distance) {
                min_distance = current_distance;
                *point = intersection_point;
                intersection_object = object;
            }
        }
    }

    return intersection_object;
}

sf::Color ray_tracer::get_screen_color_(const vector3d& screen_point) const {
    ray screen_ray(screen_point, screen_point - observer_);
    vector3d intersection_point;
    object* object_ptr = tracey_(screen_ray, &intersection_point);

    if(object_ptr != nullptr)
        return get_object_point_color_(intersection_point, object_ptr, observer_ - screen_point);


    return sf::Color();
}

long double
ray_tracer::get_point_intensity_(const vector3d& point, const object* point_object, const vector3d& dir) const {
    long double intensity = 0.1;
    vector3d normal = point_object->normal(point);
    if(scalar_product(normal, dir) < 0)
        normal = -normal;

    for(const auto& illuminant : illuminants_)
        intensity += illuminant.get_point_intensity(point, normal, objects_);

    return intensity;
}

sf::Color ray_tracer::get_object_point_color_(const vector3d& base_point, const object* const base_object,
                                              const vector3d& direction) const {
    long double base_intensity = get_point_intensity_(base_point, base_object, direction);
    long double reflection = base_object->getReflectivity();
    sf::Color base_color = base_object->get_color();

    if(reflection < 1e-9)
        return prod_(base_color, base_intensity);

    vector3d reflection_point;
    object* reflection_object = get_reflection_point_(base_point, base_object,
                                                      direction, &reflection_point);

    if(reflection_object == nullptr)
        return calculate_reflection_dist_(prod_(base_color, base_intensity),
                                          sf::Color(),
                                          base_object->getReflectivity());

    long double reflection_intensity = get_point_intensity_(reflection_point, reflection_object,
                                                            base_point - reflection_point);
    sf::Color reflection_color = reflection_object->get_color();

    return calculate_reflection_dist_(prod_(base_color, base_intensity),
                                      prod_(reflection_color, reflection_intensity),
                                      base_object->getReflectivity());
}

object* ray_tracer::get_reflection_point_(const vector3d& base_point, const object* const base_object,
                                          const vector3d& direction,
                                          vector3d* const reflection_point) const {
    vector3d normal = base_object->normal(base_point);
    if(scalar_product(normal, direction) < 0)
        normal = -normal;

    return tracey_(ray(base_point, 2 * direction.projection(normal) - direction), reflection_point);
}

sf::Color ray_tracer::calculate_reflection_dist_(const sf::Color& base_color, const sf::Color& reflection_color,
                                                 long double reflectivity) const {
    return add_(prod_(base_color, 1 - reflectivity), prod_(reflection_color, reflectivity));
}

sf::Color ray_tracer::prod_(const sf::Color& color, long double x) const {
    sf::Color result = color;

    result.r = (sf::Uint8)std::min((int)(std::round(x * (long double)(result.r))), 255);
    result.g = (sf::Uint8)std::min((int)(std::round(x * (long double)(result.g))), 255);
    result.b = (sf::Uint8)std::min((int)(std::round(x * (long double)(result.b))), 255);

    return result;
}

sf::Color ray_tracer::add_(const sf::Color& color1, const sf::Color& color2) const {
    sf::Color result = color1;

    result.r = (sf::Uint8)std::min((int)result.r + (int)color2.r, 255);
    result.g = (sf::Uint8)std::min((int)result.g + (int)color2.g, 255);
    result.b = (sf::Uint8)std::min((int)result.b + (int)color2.b, 255);

    return result;
}
