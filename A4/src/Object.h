#pragma once

#ifndef OBJECT_H
#define OBJECT_H
#include <stdlib.h>  
#include <time.h> 
#include <memory>

class Object {
    public:
        Object(int mat_count, float x_bound, float z_bound, float scale_bound, std::shared_ptr<Shape> s) {
            mat = rand() % mat_count;
            x = rand() % (int)x_bound;
            z = -1 * (rand() % (int)z_bound);
            scale = rand() % (int)scale_bound;
            y = scale/2.0f;
            shape = s;
        }
        

        int mat;
        float x;
        float z;
        float y;
        float rotation;
        float scale;
        std::shared_ptr<Shape> shape;
};

#endif