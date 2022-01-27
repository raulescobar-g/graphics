#include "Triangle.h"
#include <vector>
#include <assert.h>

BoundingBox::BoundingBox(std::vector<int>& v1,std::vector<int>& v2,std::vector<int>& v3) {
    lower.x = v1.size() > 0 ? std::min(v1[0], std::min(v2[0], v3[0])) : 0;
    lower.y = v1.size() > 1 ? std::min(v1[1], std::min(v2[1], v3[1])) : 0;
    lower.z = v1.size() > 2 ? std::min(v1[2], std::min(v2[2], v3[2])) : 0;

    upper.x = v1.size() > 0 ? std::max(v1[0], std::max(v2[0], v3[0])) : 0;
    upper.y = v1.size() > 1 ? std::max(v1[1], std::max(v2[1], v3[1])) : 0;
    upper.z = v1.size() > 2 ? std::max(v1[2], std::max(v2[2], v3[2])) : 0;
}

Position::Position(std::vector<int>& pos) {
    x = pos.size() > 0 ? pos[0] : 0;
    y = pos.size() > 1 ? pos[1] : 0;
    z = pos.size() > 2 ? pos[2] : 0;
}

Color::Color(std::vector<int>& color){
    assert(color.size() == 3);
    r = color[0];
    g = color[1];
    b = color[3];
}


Triangle::Triangle(std::vector<int>& v1, std::vector<int>& v2, std::vector<int>& v3, std::vector<int>& c1, std::vector<int>& c2, std::vector<int>& c3): v1(v1,c1),v2(v2,c2),v3(v3,c3), bb(v1,v2,v3) {}



