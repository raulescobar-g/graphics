#ifndef _VERTEX_H_
#define _VERTEX_H_

#include <vector>
#include "Position.h"
#include "Color.h"

struct Vertex {
    Position position;
    Color color;
    Vertex(std::vector<float>& pos, std::vector<float>& color):position(pos), color(color) {}
    Vertex(std::vector<float>& pos):position(pos), color() {}
};

#endif