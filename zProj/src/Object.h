#pragma once

#ifndef OBJECT_H
#define OBJECT_H
#include <stdlib.h>  
#include <time.h> 
#include <memory>

#define X_BOUND 10.0f
#define Z_BOUND 10.0f
#define SCALE_BOUND 0.5f
#define RANDOM_RESOLUTION 1000.0f
#define SPIN_SPEED 1.0

class Object {
    public: 
        Object(std::shared_ptr<Material> m, std::shared_ptr<Shape> s) {
            scale = 1;
            shape = s;
            material = m;
        }

        int scale;
        std::shared_ptr<Material> material;
        std::shared_ptr<Shape> shape;
};

#endif