#include "rt_parser.h"

int main()
{
    std::ios_base::sync_with_stdio(false);
    rt_parser parser("../samples/sample2.rt");
    parser.parse();
    parser.render();
    return 0;
}