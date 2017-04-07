#include "rt_parser.h"
#include <iostream>


int main()
{
    std::ios_base::sync_with_stdio(false);
    rt_parser parser("../samples/sample4.rt");
    parser.parse();
    parser.render();
    return 0;
}