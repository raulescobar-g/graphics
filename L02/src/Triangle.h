#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <string>
#include <vector>

struct Position {
    int x;
    int y;
    int z;
    Position(): x(0), y(0), z(0) {}
    Position(std::vector<int>& pos);
    Position(int x, int y, int z): x(x), y(y), z(z) {}
};

struct BoundingBox {
    BoundingBox(std::vector<int>&,std::vector<int>&,std::vector<int>&);
    Position lower;
    Position upper;
};

struct Color {
    int r;
    int g;
    int b;
    Color(): r(0), g(0), b(0) {}
    Color(std::vector<int>& color);
    Color(int r, int g, int b): r(r), g(g), b(b) {}
};

struct Vertex {
    Position position;
    Color color;
    Vertex(std::vector<int>& pos, std::vector<int>& color):position(pos), color(color) {}
};

class Triangle {
    public:
        Triangle(std::vector<int>&,std::vector<int>&,std::vector<int>&,std::vector<int>&,std::vector<int>&,std::vector<int>&);
        
        Vertex v1;
        Vertex v2;
        Vertex v3;

        BoundingBox bb;
};

#endif