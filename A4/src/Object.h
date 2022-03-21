#pragma once

#ifndef OBJECT_H
#define OBJECT_H
#include <stdlib.h>  
#include <time.h> 
#include <memory>

#define X_BOUND 100.0f
#define Z_BOUND 100.0f
#define SCALE_BOUND 5.0f
#define RANDOM_RESOLUTION 1000

class Object {
    public: // these objects are completely random, random material, random rotation, random position, random shape
        Object(std::shared_ptr<Material> m, std::shared_ptr<Shape> s) {
            rotation = ((rand() % ((int) (2*glm::pi<float>() * RANDOM_RESOLUTION)-1)) + 1) / (RANDOM_RESOLUTION);
            
            scale = ((rand() % ((int) (SCALE_BOUND * RANDOM_RESOLUTION)-1)) + 1) / RANDOM_RESOLUTION;
            amplitude = scale/3.0f;
            x = ((rand() % ((int) (X_BOUND * RANDOM_RESOLUTION)-1)) + 1) / RANDOM_RESOLUTION;
            z = -1 * (rand() % ((int) (Z_BOUND  * RANDOM_RESOLUTION)-1) + 1) / RANDOM_RESOLUTION;
            y = s->get_id() == "teapot" ? scale * 0.25f : scale * 0.5f; // this was very weird and im not sure why the teapot shape needs to be moved up only by 1/4 of the scaling, I thought all shapes would only need to be moved up by half the scaling when normalized to a unit box volume

            shape = s;
            material = m;
        }

        void update(float t) {
            scale = (2.0f*amplitude) + amplitude * cos(t); //maximum size is the scaled size and smallest size is 1/3 of the scaled size
            y = shape->get_id() == "teapot" ? scale * 0.25f : scale * 0.5f; //updates the y position to stick to the floor
        }
        

        
        float x;
        float z;
        float y;

        float rotation;
        float scale;
        float amplitude;
        std::shared_ptr<Material> material;
        std::shared_ptr<Shape> shape;
};

#endif