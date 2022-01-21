#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <string>
#include <vector>

struct BoundingBox {
    BoundingBox(int dims);

    std::vector<int> lower;
    std::vector<int> upper;
};

class Triangle {
    public:
        Triangle(int dims);
        Triangle(std::vector<int>&, std::vector<int>&, std::vector<int>&);
        ~Triangle();
        
        std::vector<int> v1;
        std::vector<int> v2;
        std::vector<int> v3;

        BoundingBox * bb;
};

#endif