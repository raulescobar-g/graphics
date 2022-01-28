#ifndef _COLOR_H_
#define _COLOR_H_

#include <vector>
#include <assert.h>

struct Color {
    int r;
    int g;
    int b;
    Color(): r(0), g(0), b(0) {}
    Color(std::vector<int>& color){
        assert(color.size() == 3);
        r = color[0];
        g = color[1];
        b = color[3];
    }
    Color(int r, int g, int b): r(r), g(g), b(b) {}
};

#endif