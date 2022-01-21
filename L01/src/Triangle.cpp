#include "Triangle.h"
#include <vector>

BoundingBox::BoundingBox(int dims): lower(dims, 0), upper(dims, 0) {}

Triangle::Triangle(int dims): v1(dims,0),v2(dims,0),v3(dims,0) {
    bb = new BoundingBox(dims);
}

Triangle::Triangle(std::vector<int>& v1, std::vector<int>& v2, std::vector<int>& v3): v1(v1),v2(v2),v3(v3) {
    bb = new BoundingBox(v1.size());
    
    for (int i = 0; i < v1.size(); ++i){
        bb->lower[i] = std::min(v1[i], std::min(v2[i], v3[i]));
        bb->upper[i] = std::max(v1[i], std::max(v2[i], v3[i]));
    }
}

Triangle::~Triangle() {
    delete bb;
}


