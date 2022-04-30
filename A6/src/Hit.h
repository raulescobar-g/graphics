#pragma  once
#ifndef HIT_H
#define HIT_H


#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

#include "Object.h"

class Hit {
    public:

        Hit(glm::vec3 n, float s, Object* _object) : normal(n), ds(s), object(_object){}; 

        glm::vec3 normal;
        float ds;
        Object * object;
};

#endif