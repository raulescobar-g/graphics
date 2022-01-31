#ifndef _ZBUFFER_H_
#define _ZBUFFER_H_

#include <vector>
#include "assert.h"


class Zbuff {
    private:
        int width;
        int height;
        std::vector<float> m;
    public:
        Zbuff(int w, int h) : width(w), height(h), m(width*height, -std::numeric_limits<float>::infinity()) {}

        void set_ds(int x,int y,float d) {
            assert(y >= 0 && y < height);
            assert(x >= 0 && x < width);
            
            y = height - y - 1;
            m[y * width + x] = d;
        }

        float get_ds(int x,int y){
            assert(y >= 0 && y < height);
            assert(x >= 0 && x < width);
            
            y = height - y - 1;
            return m[y * width + x];
        }

};



#endif