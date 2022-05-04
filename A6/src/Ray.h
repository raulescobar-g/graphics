#pragma  once
#ifndef RAY_H
#define RAY_H

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Hit.h"
#include "Object.h"

class Ray {
    public: 
        Ray(float _x, float _y, float _z, glm::vec3 o){
            direction = glm::normalize(glm::vec3(_x,_y,_z));
            origin = o;
        };
        Ray(glm::vec3 dir, glm::vec3 o){
            direction = glm::normalize(dir);
            origin = o;
        };

        glm::vec3 xyz() {return direction;};
        glm::vec3 o() { return origin;};
    private:
        glm::vec3 direction;
        glm::vec3 origin;
};

#endif