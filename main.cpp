#include "rt_parser.h"
#include <iostream>


void draw2() {
    vector3d observer(0, 0, 0);

    screen screen1(vector3d(-10, 10, 10), vector3d(1, 0, 0), vector3d(0, 0, -1), 20, 20);

    std::vector<object*> objects(2);
    objects[0] = new sphere(sf::Color(0, 0, 255), 0, 0, 0, vector3d(0, 20, 0), 5);
    objects[1] = new quadrilateral(sf::Color::White, 0, 0, 0.5, vector3d(10, -30, -30), vector3d(10, -30, 30),
                                   vector3d(10, 30, 30),
                                   vector3d(10, 30, -30));

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
    rt_parser parser("../samples/sample3.rt");
    parser.parse();
    parser.render();
    return 0;
}