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

class Object {
    public: 
        // these objects are completely random, random material, random rotation, random position, random shape
        Object(std::shared_ptr<Material> m, std::shared_ptr<Shape> s) {

            rotation = ((rand() % ((int) (2*glm::pi<float>() * RANDOM_RESOLUTION)-1)) + 1) / (RANDOM_RESOLUTION);
            axis = glm::vec3(0.0f,1.0f,0.0f);
            
            scale = ((rand() % ((int) (SCALE_BOUND * RANDOM_RESOLUTION)-1)) + 1) / RANDOM_RESOLUTION ;
            amplitude = scale/3.0f;

            x = (((rand() % ((int) (X_BOUND * RANDOM_RESOLUTION)-1)) + 1) / RANDOM_RESOLUTION) - (X_BOUND/2.0f);
            z = ((rand() % ((int) (Z_BOUND  * RANDOM_RESOLUTION)-1) + 1) / RANDOM_RESOLUTION) - (Z_BOUND/2.0f);

            y = s->get_id() == "teapot" ? scale * 0.25f : scale * 0.5f; // this was very weird and im not sure why the teapot shape needs to be moved up only by 1/4 of the scaling, I thought all shapes would only need to be moved up by half the scaling when normalized to a unit box volume
            shape = s;
            material = m;
        }

        // defined objects for the ground
        Object(std::shared_ptr<Material> m, std::shared_ptr<Shape> s, glm::vec3 position, float scale, glm::vec3 axis) {
            material = m;
            shape = s;

            this->scale = scale;

            x = position.x;
            y = position.y;
            z = position.z;
            
            amplitude = -1.0f;

            rotation = glm::pi<float>()/2.0f;
            this->axis = axis;
        }

        void spin(float radians) {
            rotation += radians;
        }
        
        float x;
        float z;
        float y;

        float rotation;
        glm::vec3 axis;

        float scale;
        float amplitude;

        std::shared_ptr<Material> material;
        std::shared_ptr<Shape> shape;
};

#endif