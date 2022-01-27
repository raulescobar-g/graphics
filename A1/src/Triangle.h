#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <string>
#include <vector>
#include "Vertex.h"
#include "BoundingBox.h"

class Triangle {
    public:
        Triangle(std::vector<float>& v1, std::vector<float>& v2, std::vector<float>& v3, std::vector<float>& c1, std::vector<float>& c2, std::vector<float>& c3): v1(v1,c1),v2(v2,c2),v3(v3,c3), bb(v1,v2,v3) {};
        Triangle(std::vector<float>& v1,std::vector<float>& v2,std::vector<float>& v3): v1(v1),v2(v2),v3(v3), bb(v1,v2,v3) {};
        Vertex v1;
        Vertex v2;
        Vertex v3;

        BoundingBox bb;
};

#endif