#ifndef _POSITION_H_
#define _POSITION_H_

#include <vector>

struct Position {
    float x;
    float y;
    float z;
    Position(): x(0.0), y(0.0), z(0.0) {}

    Position(std::vector<float>& pos){
        x = pos.size() > 0 ? pos[0] : 0.0;
        y = pos.size() > 1 ? pos[1] : 0.0;
        z = pos.size() > 2 ? pos[2] : 0.0;
    }
    
    Position(float x, float y, float z): x(x), y(y), z(z) {}
};

#endif