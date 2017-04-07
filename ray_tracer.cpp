//
// Created by randan on 4/5/17.
//

#include "ray_tracer.h"

ray_tracer::ray_tracer(const std::vector<object*>& objects,
                       const std::vector<illuminant>& illuminants)
          : objects_(objects)
          , illuminants_(illuminants) { }

void ray_tracer::render(const screen& screen_to_render, const vector3d& observer, unsigned int axis1_resolution,
                        unsigned int axis2_resolution, unsigned int aa_coef) {
    screen_ = screen_to_render;
    observer_ = observer;
    true_axis1_resolution_ = axis1_resolution;
    true_axis2_resolution_ = axis2_resolution;
    render_axis1_resolution_ = axis1_resolution * aa_coef;
    render_axis2_resolution_ = axis2_resolution * aa_coef;

    screen_.set_resolution(render_axis1_resolution_, render_axis2_resolution_);
    sf::VertexArray render_screen = sf::VertexArray(sf::Points,
                                                    render_axis1_resolution_ * render_axis2_resolution_);
    set_screen_position_(render_screen, render_axis1_resolution_, render_axis2_resolution_);

    pixel_number_ = 0;
    unsigned int n = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    for(unsigned int i = 0; i < n - 1; ++i)
        threads.emplace_back(&ray_tracer::sub_render_, this, std::ref(render_screen));

    sub_render_(render_screen);

    for(auto& thread : threads)
        thread.join();

    anti_aliasing_(render_screen, aa_coef);
}

void ray_tracer::draw() const {
    sf::RenderWindow window(sf::VideoMode(true_axis1_resolution_, true_axis2_resolution_), "ray_tracer");

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

object* ray_tracer::trace_(const ray& ray, vector3d* const point) const {
    double min_distance = 1e18;
    object* intersection_object = nullptr;

    for(auto object : objects_) {
        vector3d intersection_point;
        if(object->intersect(ray, &intersection_point)){
            double current_distance = (intersection_point - ray.origin_).module();
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
    ray ray(screen_point, screen_point - observer_);
    return get_color_(ray, 3);
}

sf::Color ray_tracer::get_color_(const ray& cur_ray, unsigned int depth) const {
    vector3d intersection_point;
    object* object_ptr = trace_(cur_ray, &intersection_point);
    if (!object_ptr)
        return sf::Color();
    auto normal = object_ptr->normal(intersection_point);

    double intensity = get_point_intensity_(intersection_point, object_ptr, cur_ray.origin_ - intersection_point);
    if (depth == 0) {
        return prod_(object_ptr->get_color(), intensity);
    }

    double reflection = object_ptr->getReflectivity();
    sf::Color mirrored_color;
    if (reflection > 0) {
        ray mirrored_ray(cur_ray.mirror(intersection_point, normal));
        mirrored_color = get_color_(mirrored_ray, depth - 1);
    }

    sf::Color base = prod_(object_ptr->get_color(), intensity);
    sf::Color refraction;
    if (object_ptr->getAlpha() != 1) {
        double cos_phi = scalar_product(cur_ray.direction_, normal);
        double k; // outside / inside
        if (cos_phi < 0) {
            cos_phi = -cos_phi;
            k = object_ptr->getRefractiviry();
        } else {
            k = 1 / object_ptr->getRefractiviry();
            normal = -normal;
        }

        double cos2_psi = 1 - k * k * (1 - cos_phi * cos_phi);
        if (cos2_psi > 0) {
            auto refracted = k * cur_ray.direction_ + (cos_phi * k - std::sqrt(cos2_psi)) * normal;
            ray refracted_ray(intersection_point, refracted);
            refraction = get_color_(refracted_ray, depth - 1);
        } else {
            refraction = mirrored_color;
        }
    } else
        refraction = sf::Color();


    base = calculate_dist_color_(refraction, base, object_ptr->getAlpha());
    return calculate_dist_color_(base, mirrored_color, reflection);
}

double
ray_tracer::get_point_intensity_(const vector3d& point, const object* point_object, const vector3d& dir) const {
    double intensity = 0.1;
    vector3d normal = point_object->normal(point);
    if(scalar_product(normal, dir) < 0)
        normal = -normal;

    for(const auto& illuminant : illuminants_)
        intensity += illuminant.get_point_intensity(point, normal, objects_);

    return intensity;
}

sf::Color ray_tracer::calculate_dist_color_(const sf::Color& base_color, const sf::Color& reflection_color,
                                            double reflectivity) const {
    return add_(prod_(base_color, 1 - reflectivity), prod_(reflection_color, reflectivity));
}

sf::Color ray_tracer::prod_(const sf::Color& color, double x) const {
    sf::Color result = color;

    result.r = (sf::Uint8)std::min((int)(std::round(x * (double)(result.r))), 255);
    result.g = (sf::Uint8)std::min((int)(std::round(x * (double)(result.g))), 255);
    result.b = (sf::Uint8)std::min((int)(std::round(x * (double)(result.b))), 255);

    return result;
}

sf::Color ray_tracer::add_(const sf::Color& color1, const sf::Color& color2) const {
    sf::Color result = color1;

    result.r = (sf::Uint8)std::min((int)result.r + (int)color2.r, 255);
    result.g = (sf::Uint8)std::min((int)result.g + (int)color2.g, 255);
    result.b = (sf::Uint8)std::min((int)result.b + (int)color2.b, 255);

    return result;
}

void ray_tracer::sub_render_(sf::VertexArray& render_screen) {
    while(true) {
        unsigned int pixel = pixel_number_++;

        if(pixel >= render_axis1_resolution_ * render_axis2_resolution_)
            return;

        unsigned int step1 = pixel / render_axis2_resolution_;
        unsigned int step2 = pixel % render_axis2_resolution_;
        render_screen[pixel].color =
                get_screen_color_(screen_.origin_ + step1 * screen_.dir1_ + step2 * screen_.dir2_);
    }
}

void ray_tracer::set_screen_position_(sf::VertexArray& render_screen, unsigned int axis1_resolution,
                                      unsigned int axis2_resolution) {
    for(std::size_t  i = 0; i < axis1_resolution; ++i)
        for(std::size_t j = 0; j < axis2_resolution; ++j)
            render_screen[i * axis2_resolution + j].position = sf::Vector2f(i, j);
}

void ray_tracer::anti_aliasing_(const sf::VertexArray& screen, unsigned int aa_coef) {
    if(aa_coef == 1) {
        real_screen_ = screen;
        return;
    }
    real_screen_ = sf::VertexArray(sf::Points, true_axis1_resolution_ * true_axis2_resolution_);
    set_screen_position_(real_screen_, true_axis1_resolution_, true_axis2_resolution_);

    pixel_number_ = 0;
    unsigned int n = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;

    for(unsigned int i = 0; i < n - 1; ++i)
        threads.emplace_back(&ray_tracer::sub_antialiasing_, this, screen, aa_coef);

    sub_antialiasing_(screen, aa_coef);

    for(auto& thread : threads)
        thread.join();
}

void ray_tracer::sub_antialiasing_(const sf::VertexArray& screen, unsigned int aa_coef) {
    while(true) {
        unsigned int pixel = pixel_number_++;

        if(pixel >= true_axis1_resolution_ * true_axis2_resolution_)
            return;

        unsigned int step1 = pixel / true_axis2_resolution_;
        unsigned int step2 = pixel % true_axis2_resolution_;
        real_screen_[pixel].color =
                get_pixel_color_(screen, aa_coef, step1, step2);
    }

}

sf::Color ray_tracer::get_pixel_color_(const sf::VertexArray& screen, unsigned int aa_coef, const unsigned int x,
                                       const unsigned int y) const {
    int r = 0;
    int g = 0;
    int b = 0;
    for(unsigned int i = x * aa_coef; i < (x + 1) * aa_coef; ++i)
        for(unsigned int j = y * aa_coef; j < (y + 1) * aa_coef; ++j){
            r += screen[i * render_axis2_resolution_ + j].color.r;
            g += screen[i * render_axis2_resolution_ + j].color.g;
            b += screen[i * render_axis2_resolution_ + j].color.b;
        }
    return sf::Color((sf::Uint8)(r / aa_coef / aa_coef),
                     (sf::Uint8)(g /aa_coef / aa_coef) ,
                     (sf::Uint8)(b / aa_coef / aa_coef));
}
