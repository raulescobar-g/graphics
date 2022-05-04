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
        Ellipsoid(glm::vec3 pos, glm::vec3 scale,float theta,glm::vec3 axis, std::shared_ptr<Material> material) : Object(pos,scale,theta,axis,material) {
            M = glm::translate(glm::mat4(1.0f), pos) * glm::rotate(glm::mat4(1.0f), theta, axis) * glm::scale(glm::mat4(1.0f), scale);
            iM = glm::inverse(M);
            Mt = glm::transpose(M);
            iMt = glm::transpose(iM);
        }

        Hit intersect(std::shared_ptr<Ray> ray, float start, float stop){
            Hit hit = Hit();

            glm::vec3 s_direction = glm::normalize((iM * glm::vec4(ray->xyz(), 0.0f))); // v
            glm::vec3 s_origin = (iM * glm::vec4(ray->o() + (ray->xyz() * start), 1.0f));       // p

            float a = glm::dot( s_direction, s_direction);
            float b = 2.0f * (glm::dot(s_origin, s_direction));
            float c = glm::dot(s_origin,s_origin) - 1.0f;

            float d = b*b - 4 * a * c;
            if (d <= 0) return hit;

            float t1 = (-b + sqrt(d)) / (2 * a);
            float t2 = (-b - sqrt(d)) / (2 * a);

            glm::vec3 x1_p = (s_direction * t1) + s_origin;
            glm::vec3 x2_p = (s_direction * t2) + s_origin;

            glm::vec3 x1 = glm::vec3(M * glm::vec4(x1_p, 1.0f));
            glm::vec3 x2 = glm::vec3(M * glm::vec4(x2_p, 1.0f));


            glm::vec3 n1 = glm::normalize( glm::vec3( iMt * glm::vec4(glm::normalize(x1_p), 0.0f) ) );
            glm::vec3 n2 = glm::normalize( glm::vec3( iMt * glm::vec4(glm::normalize(x2_p), 0.0f) ) );

            Material mat = *material.get();

            t1 = glm::dot(ray->xyz(), x1 - ray->o()) < 0.0f ? -glm::length(x1 - ray->o()) : glm::length(x1 - ray->o());
            t2 = glm::dot(ray->xyz(), x2 - ray->o()) < 0.0f ? -glm::length(x2 - ray->o()) : glm::length(x2 - ray->o());

            Hit hitA(n1, t1, ray, mat);
            Hit hitB(n2, t2, ray, mat);

            if (t1 > 0.0f && t1 < stop && t1 < t2) {
                return hitA;
            } else if (t2 > 0.0f && t2 < stop && t2 < t1) {
                return hitB;
            } else {
                return hit;
            }
        }
    private:
        glm::mat4 M;
        glm::mat4 iM;
        glm::mat4 Mt;
        glm::mat4 iMt;
            
};


#endif