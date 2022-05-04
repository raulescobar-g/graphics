#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Object.h"
#include "Hit.h"

class Sphere : public Object {
    public:
       using Object::Object;

        Hit intersect(std::shared_ptr<Ray> ray, float start, float stop){
            
            Hit hit = Hit();
        
            glm::vec3 offset_o = ray->o() + (start * ray->xyz());

            float a = glm::dot( ray->xyz(), ray->xyz());
            float b = 2.0f * (glm::dot((offset_o - pos), ray->xyz()));
            float c = glm::dot(offset_o - pos,offset_o - pos) - scale[0]*scale[0];
                        
            float d = b*b - 4 * a * c;
            if (d <= 0.0f) return hit;

            float t1 = (-b + sqrt(d)) / (2 * a);
            float t2 = (-b - sqrt(d)) / (2 * a);

            if ((t1 > stop && t2 > stop) || (t2 < 0.0f && t1 < 0.0f)) return hit;
            
            glm::vec3 n1 = ((offset_o + (ray->xyz() * t1)) - pos) / scale[0];
            glm::vec3 n2 = ((offset_o + (ray->xyz() * t2)) - pos) / scale[0];
            Material mat = *material.get();

            // very annoying logic
            if (t1 < stop && t1 > 0.0f && t1 < t2){
                return Hit(n1, t1, ray, mat);
            } else if (t2 < stop && t2 > 0.0f && t2 < t1){
                return Hit(n2, t2, ray, mat);
            } else {
                return hit;
            }
        }

};


#endif