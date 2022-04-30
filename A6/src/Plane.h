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

       void intersections(glm::vec3 origin, glm::vec3 direction, std::vector<Hit>& hitBuff){ //SCALAR IS THE NORMAL OF PLANE IN THIS CASE

            if (glm::dot(scale, direction) <= 0.000001f && glm::dot(scale, direction) >= -0.000001f) { return; }
            float t = glm::dot(scale , pos - origin) / (glm::dot(scale, direction)) ;

            if (t <= 0.0f) {return;}

            Hit hit(scale, t, this);
            
            if (hitBuff.empty()) {
                hitBuff.push_back(hit);
            } else if (hitBuff[0].ds > t) {
                hitBuff[0] = hit;
            }
        }

        void intersections(glm::vec3 origin, glm::vec3 direction, std::vector<Hit>& hitBuff, float dist){

            if (glm::dot(scale, direction) <= 0.000001f && glm::dot(scale, direction) >= -0.000001f) { return; }
            float t = glm::dot(scale , pos - (origin + direction*0.0001f)) / (glm::dot(scale, direction)) ;
            if (t <= 0.0f) {return;}
            Hit hit(scale, t, this);
            
            if (t < dist) {
                hitBuff.push_back(hit);
            }
            
        }

};


#endif