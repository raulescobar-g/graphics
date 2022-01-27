#ifndef _BOUNDINGBOX_H_
#define _BOUNDINGBOX_H_

#include <vector>
#include "Position.h"

struct BoundingBox {
    BoundingBox(): lower(INFINITY,INFINITY , INFINITY), upper(-1.0 * INFINITY, -1.0 * INFINITY, -1.0 * INFINITY) {};
    BoundingBox(std::vector<float>& v1,std::vector<float>& v2,std::vector<float>& v3){
        lower.x = v1.size() > 0 ? std::min(v1[0], std::min(v2[0], v3[0])) : 0;
        lower.y = v1.size() > 1 ? std::min(v1[1], std::min(v2[1], v3[1])) : 0;
        lower.z = v1.size() > 2 ? std::min(v1[2], std::min(v2[2], v3[2])) : 0;

        upper.x = v1.size() > 0 ? std::max(v1[0], std::max(v2[0], v3[0])) : 0;
        upper.y = v1.size() > 1 ? std::max(v1[1], std::max(v2[1], v3[1])) : 0;
        upper.z = v1.size() > 2 ? std::max(v1[2], std::max(v2[2], v3[2])) : 0;
    }
    
    Position lower;
    Position upper;
};

#endif