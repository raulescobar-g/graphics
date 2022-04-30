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
        // these objects are completely random, random material, random rotation, random position, random shape
        Object(std::shared_ptr<Material> m, std::shared_ptr<Shape> s) {

            if (s->get_id() == "vase"){
                rotation = glm::pi<float>() / 2.0f;
                axis = glm::vec3(0.0f, 0.0f, 1.0f);
            } else {
                rotation = ((rand() % ((int) (2*glm::pi<float>() * RANDOM_RESOLUTION)-1)) + 1) / (RANDOM_RESOLUTION);
                axis = glm::vec3(0.0f,1.0f,0.0f);
            }
            
            float scalar = ((rand() % ((int) (SCALE_BOUND * RANDOM_RESOLUTION)-1)) + 1) / RANDOM_RESOLUTION ;
            scalar *= s->get_id() == "vase" ? 0.1f : 1.0f;

            scale = glm::vec3(scalar,scalar,scalar);
            deformation = glm::vec3(0.0f, 0.0f, 0.0f);

            position.x = (((rand() % ((int) (X_BOUND * RANDOM_RESOLUTION)-1)) + 1) / RANDOM_RESOLUTION) - (X_BOUND/2.0f);
            position.y = s->get_id() == "teapot" ? scalar * 0.25f : scalar * 0.5f; // this needs to be fixed for gods sake
            position.z = ((rand() % ((int) (Z_BOUND  * RANDOM_RESOLUTION)-1) + 1) / RANDOM_RESOLUTION) - (Z_BOUND/2.0f);
            
            wiggle = glm::vec3(0.0f, 0.0f, 0.0f);

            shape = s;
            material = m;

            shearX = glm::vec2(0.0f, 0.0f);
            shearY = glm::vec2(0.0f, 0.0f);
            shearZ = glm::vec2(0.0f, 0.0f);
        }

        // defined objects for the ground
        Object(std::shared_ptr<Material> m, std::shared_ptr<Shape> s, glm::vec3 _position, float scalar, glm::vec3 axis) {
            material = m;
            shape = s;

            scale = glm::vec3(scalar,scalar,scalar);
            deformation = glm::vec3(0.0f, 0.0f, 0.0f);
            position = _position;

            wiggle = glm::vec3(0.0f, 0.0f, 0.0f);

            rotation = glm::pi<float>()/2.0f;
            this->axis = axis;

            shearX = glm::vec2(0.0f, 0.0f);
            shearY = glm::vec2(0.0f, 0.0f);
            shearZ = glm::vec2(0.0f, 0.0f);
        }


        void update_object(float t, float dt) {
            std::string id = shape->get_id();
            
            if (id == "bunny"){
                rotation += SPIN_SPEED * dt;
            } else if (id == "teapot") {
                float shear_q = 1.0f;
                wiggle.x = (shear_q / 8.0f)*cos(t*2.0f);
                shearX.x = shear_q*cos(t*2.0);
            } else if (id == "gen_sphere") {
                float Ay = 0.3f, As = 0.5f, p = 1.7f, t0 = 0.9f;
                wiggle.y = Ay * (0.5f * sin((2.0f * glm::pi<float>() / p) * (t + t0)) + 0.5f);
                deformation.x = -As * (0.5f * cos((4.0f * glm::pi<float>() / p) * (t + t0)) + 0.5f) * scale.x;
                deformation.z = -As * (0.5f * cos((4.0f * glm::pi<float>() / p) * (t + t0)) + 0.5f) * scale.z;
            } else {
                return;
            }
        }

        
        glm::vec3 position; // from center of mass 
        glm::vec3 wiggle;
        glm::vec3 axis;
        glm::vec3 scale;
        glm::vec3 deformation;

        glm::vec2 shearX;
        glm::vec2 shearY;
        glm::vec2 shearZ;

        float rotation;

        std::shared_ptr<Material> material;
        std::shared_ptr<Shape> shape;
};

#endif