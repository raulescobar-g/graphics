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
        Ray(float _x, float _y, float _z, glm::vec3 o, float s=-1.0f){
            glm::vec3 temp = glm::normalize(glm::vec3(_x,_y,_z));
            x = temp.x;
            y = temp.y;
            z = temp.z;
            origin = o;
            scalar = s;
        };

        glm::vec3 get_ray() {
            return glm::vec3(x,y,z);
        }
        void debug() {
            std::cout<<"<"<<x<<", "<<y<<", "<<z<<">"<<std::endl;
        }

        void hits(std::vector< std::shared_ptr<Object> >& objects, std::vector<Hit>& result) {
            for (auto obj : objects){
                obj->intersections(origin, glm::vec3(x,y,z), result); // populates result vector inline
            }
            if (result.size() > 1) {std::cout<<"pow wow"<<std::endl;}
        }

        void light_hits(std::vector< std::shared_ptr<Object> >& objects, std::vector<Hit>& result) {
            for (auto obj : objects){
                obj->intersections(origin, glm::vec3(x,y,z), result, scalar); // populates result vector inline
                if (result.size() > 0) {return;}
            }
        }

        glm::vec3 get_origin() { return origin;};
    private:
        float x;
        float y;
        float z;

        glm::vec3 origin;
        float scalar;
};

#endif