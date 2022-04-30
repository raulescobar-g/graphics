#pragma once

#ifndef ELLIPSOID_H
#define ELLIPSOID_H

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Object.h"
#include "Hit.h"

class Ellipsoid : public Object {
    public:
        using Object::Object;

        void intersections(glm::vec3 origin, glm::vec3 direction, std::vector<Hit>& hitBuff){

            glm::vec3 s_direction = glm::normalize((iM * glm::vec4(glm::normalize(direction), 0.0f))); // v
            glm::vec3 s_origin = (iM * glm::vec4(origin, 1.0f));       // p

            float a = glm::dot( s_direction, s_direction);
            float b = 2.0f * (glm::dot(s_origin, s_direction));
            float c = glm::dot(s_origin,s_origin) - 1.0f;

            float d = b*b - 4 * a * c;
            if (d <= 0) return;

            float t1 = (-b + sqrt(d)) / (2 * a);
            float t2 = (-b - sqrt(d)) / (2 * a);

            glm::vec3 x1 = (glm::vec3)(M * glm::vec4(s_direction * t1 + s_origin, 1.0f));
            glm::vec3 x2 = (glm::vec3)(M * glm::vec4(s_direction * t2 + s_origin, 1.0f));


            glm::vec3 n1 = (s_origin + (s_direction * t1));
            glm::vec3 n2 = (s_origin + (s_direction * t2));

            Hit hitA(glm::normalize(Mt * glm::vec4(n1, 0.0f)), glm::length(x1), this);
            Hit hitB(glm::normalize(Mt * glm::vec4(n2, 0.0f)), glm::length(x2), this);

            
            if (glm::length(x1) > 0) {
                if (hitBuff.empty()) {
                    hitBuff.push_back(hitA);
                } else if (hitBuff[0].ds > glm::length(x1)){
                    hitBuff[0] = hitA;
                }
            }
            if (glm::length(x2) > 0) {
                if (hitBuff.empty()) {
                    hitBuff.push_back(hitB);
                } else if (hitBuff[0].ds > glm::length(x2)){
                    hitBuff[0] = hitB;
                }
            } 
        }

        void intersections(glm::vec3 origin, glm::vec3 direction, std::vector<Hit>& hitBuff, float scalar){
            glm::vec3 s_direction = glm::normalize((iM * glm::vec4(glm::normalize(direction), 0.0f))); // v
            glm::vec3 s_origin = (iM * glm::vec4(origin+direction*0.0001f, 1.0f));       // p

            float a = glm::dot( s_direction, s_direction);
            float b = 2.0f * (glm::dot(s_origin, s_direction));
            float c = glm::dot(s_origin,s_origin) - 1.0f;

            float d = b*b - 4 * a * c;
            if (d <= 0) return;

            float t1 = (-b + sqrt(d)) / (2 * a);
            float t2 = (-b - sqrt(d)) / (2 * a);

            glm::vec3 x1 = (glm::vec3)(M * glm::vec4(s_direction * t1 + s_origin, 1.0f));
            glm::vec3 x2 = (glm::vec3)(M * glm::vec4(s_direction * t2 + s_origin, 1.0f));


            glm::vec3 n1 = (s_origin + (s_direction * t1));
            glm::vec3 n2 = (s_origin + (s_direction * t2));

            Hit hitA(glm::normalize(Mt * glm::vec4(n1, 0.0f)), glm::length(x1), this);
            Hit hitB(glm::normalize(Mt * glm::vec4(n2, 0.0f)), glm::length(x2), this);

            
            if (glm::length(x1) > 0 && glm::length(x1) < scalar) {
                hitBuff.push_back(hitA);
            }
            if (glm::length(x2) > 0 && glm::length(x2) < scalar) {
                hitBuff.push_back(hitB);
            }
        }
            
};


#endif