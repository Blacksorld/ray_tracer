#include "rt_parser.h"
#include <iostream>

void draw1() {
    vector3d observer(0, 0, 0);

    screen screen1(vector3d(-10, 10, 10), vector3d(1, 0, 0), vector3d(0, 0, -1), 20, 20);

    std::vector<object*> objects(4);
    objects[0] = new sphere(sf::Color(0, 0, 255), 0, vector3d(0, 20, 0), 5);
    objects[1] = new triangle(sf::Color(0, 255, 0), 0, vector3d(0, 11, 0), vector3d(3, 11, 0), vector3d(3, 15, 4));
    objects[2] = new sphere(sf::Color(255, 0, 0), 0, vector3d(5, 20, 0), 6);
    objects[3] = new quadrilateral(sf::Color::White, 0.5, vector3d(-30, -30, -6), vector3d(60, -30, -6),
                                   vector3d(60, 60, -6), vector3d(-30, 60, -6));

    std::vector<illuminant> il;
    il.push_back(illuminant(vector3d(5, 0, 0), 150));
    il.push_back(illuminant(vector3d(-10, 0, 0), 150));


    ray_tracer tracer(objects, il);

    tracer.render(screen1, observer, 800, 800, 4);
    tracer.draw();

    delete objects[0];
    delete objects[1];
    delete objects[2];
    delete objects[3];
}

void draw2() {
    vector3d observer(0, 0, 0);

    screen screen1(vector3d(-10, 10, 10), vector3d(1, 0, 0), vector3d(0, 0, -1), 20, 20);

    std::vector<object*> objects(2);
    objects[0] = new sphere(sf::Color(0, 0, 255), 0, vector3d(0, 20, 0), 5);
    objects[1] = new quadrilateral(sf::Color::White, 0.5, vector3d(10, -30, -30), vector3d(10, -30, 30),
                                   vector3d(10, 30, 30), vector3d(10, 30, -30));

    std::vector<illuminant> il;
    il.push_back(illuminant(vector3d(-10, 0, 0), 350));


    ray_tracer tracer( objects, il);

    tracer.render(screen1, observer, 800, 800, 1);
    tracer.draw();

    delete objects[0];
    delete objects[1];
}

int main()
{
    std::ios_base::sync_with_stdio(false);
//    draw1();
    rt_parser parser("sample.rt");
    parser.parse();
    parser.render();
    return 0;
}