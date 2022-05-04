#pragma once

#ifndef PLANE_H
#define PLANE_H

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Object.h"
#include "Hit.h"

class Plane : public Object {
    public:
       using Object::Object;

        Hit intersect(std::shared_ptr<Ray> ray, float start, float stop){ 
            //scale is actually normal here
            Hit hit = Hit();
            glm::vec3 origin = ray->o() + (start * ray->xyz());

            if (abs(glm::dot(scale, ray->xyz())) <= 0.000001f) { return hit; }
            float t = glm::dot(scale , pos - origin) / (glm::dot(scale, ray->xyz())) ;

            return t < stop && t > 0.0f ? Hit(scale, t, ray, *material.get()) : hit;
        }

};


#endif