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

       void intersections(glm::vec3 origin, glm::vec3 direction, std::vector<Hit>& hitBuff){

            float a = glm::dot( direction, direction);
            float b = 2.0f * (glm::dot((origin - pos), direction));
            float c = glm::dot(origin - pos,origin - pos) - scale[0]*scale[0];

            float d = b*b - 4 * a * c;
            if (d <= 0) return;

            float t1 = (-b + sqrt(d)) / (2 * a);
            float t2 = (-b - sqrt(d)) / (2 * a);


            glm::vec3 n1 = ((origin + (direction * t1)) - pos) / scale[0];
            glm::vec3 n2 = ((origin + (direction * t2)) - pos) / scale[0];

            Hit hitA(n1, t1, this);
            Hit hitB(n2, t2, this);

            
            if (t1 > 0 && (hitBuff.empty() || hitBuff[0].ds > t1)) {
                if (hitBuff.empty()) {
                    hitBuff.push_back(hitA);
                } else {
                    hitBuff[0] = hitA;
                }
            }
            if (t2 > 0 && (hitBuff.empty() || hitBuff[0].ds > t2)) {
                if (hitBuff.empty()) {
                    hitBuff.push_back(hitB);
                } else {
                    hitBuff[0] = hitB;
                }
            } 
        }

        void intersections(glm::vec3 origin, glm::vec3 direction, std::vector<Hit>& hitBuff, float scalar){
            origin = origin + direction * 1.001f;
            float a = glm::dot( direction, direction);
            float b = 2.0f * (glm::dot((origin - pos), direction));
            float c = glm::dot(origin - pos,origin - pos) - scale[0]*scale[0];

            float d = b*b - 4 * a * c;
            if (d <= 0) return;

            float t1 = (-b + sqrt(d)) / (2 * a);
            float t2 = (-b - sqrt(d)) / (2 * a);


            glm::vec3 n1 = ((origin + (direction * t1)) - pos) / scale[0];
            glm::vec3 n2 = ((origin + (direction * t2)) - pos) / scale[0];

            Hit hitA(n1, t1, this);
            Hit hitB(n2, t2, this);

            
            if (t1 >= 0 && t1 < scalar) {
                hitBuff.push_back(hitA);
            }
            if (t2 >= 0 && t2 < scalar) {
                hitBuff.push_back(hitB);
            } 
        }

};


#endif