//
// Created by randan on 4/5/17.
//

#pragma once


#include "screen.h"
#include "object.h"
#include "illuminant.h"
#include <vector>
#include <thread>
#include <atomic>

class ray_tracer {
public:
    ray_tracer(const std::vector<object*>& objects, const std::vector<illuminant>& illuminants);

    void render(const screen& screen_to_render, const vector3d& observer, unsigned int axis1_resolution,
                    unsigned int axis2_resolution, unsigned int aa_coef);

    void draw() const;

private:
    screen screen_;
    vector3d observer_;
    unsigned int render_axis1_resolution_;
    unsigned int render_axis2_resolution_;
    unsigned int true_axis1_resolution_;
    unsigned int true_axis2_resolution_;
    sf::VertexArray real_screen_;
    std::atomic<unsigned int> pixel_number_;

    std::vector<object*> objects_;
    std::vector<illuminant> illuminants_;

    object* tracey_(class ray ray, vector3d* const point) const;
    sf::Color get_screen_color_(const vector3d& screen_point) const;
    long double get_point_intensity_(const vector3d& point, const object* point_object, const vector3d& dir) const;

    sf::Color get_object_point_color_(const vector3d& base_point, const object* const base_object,
                                      const vector3d& direction) const;
    object* get_reflection_point_(const vector3d& base_point, const object* const base_object,
                                  const vector3d& direction,
                                  vector3d* const reflection_point) const;
    sf::Color calculate_reflection_dist_(const sf::Color& base_color,
                                         const sf::Color& reflection_color, long double reflectivity) const;

    sf::Color prod_(const sf::Color& color, long double x) const;
    sf::Color add_(const sf::Color& color1, const sf::Color& color2) const;

    void sub_render_(sf::VertexArray& render_screen);

    void set_screen_position_(sf::VertexArray& render_screen, unsigned int axis1_resolution,
                                  unsigned int axis2_resolution);

    void anti_aliasing_(const sf::VertexArray& screen, unsigned int aa_coef);

    void sub_antialiasing_(const sf::VertexArray& screen, unsigned int aa_coef);

    sf::Color get_pixel_color_(const sf::VertexArray& screen, unsigned int aa_coef,
                          const unsigned int x, const unsigned int y) const;
};
