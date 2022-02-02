#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vector>
#include "Position.h"
#include "Color.h"

struct Vertex {
    Position position;
    Color color;
    std::vector<float> normal;
    Vertex(std::vector<int>& pos, std::vector<int>& color):position(pos), color(color), normal() {}
    Vertex(std::vector<int>& pos):position(pos), color(), normal() {}
};

#endif