#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <string>
#include <vector>
#include "Vertex.h"
#include "BoundingBox.h"

class Triangle {
    public:
        Triangle(std::vector<int>& v1, std::vector<int>& v2, std::vector<int>& v3, std::vector<int>& c1, std::vector<int>& c2, std::vector<int>& c3): v1(v1,c1),v2(v2,c2),v3(v3,c3), bb(v1,v2,v3) {};
        Triangle(std::vector<int>& v1,std::vector<int>& v2,std::vector<int>& v3): v1(v1),v2(v2),v3(v3), bb(v1,v2,v3) {};
        Vertex v1;
        Vertex v2;
        Vertex v3;

        BoundingBox bb;
};

#endif