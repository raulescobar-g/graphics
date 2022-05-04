#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Material.h"
#include "Ray.h"
class Hit;

class Object {
    public:
        Object(glm::vec3 pos, glm::vec3 scale, float theta, glm::vec3 axis , std::shared_ptr<Material> material): pos(pos), scale(scale), material(material), theta(theta), axis(axis) {};

        virtual ~Object() {}
        virtual Hit intersect(std::shared_ptr<Ray> ray, float start, float stop){
            return Hit();
        };

        virtual std::shared_ptr<Material> get_material() { 
            return material;
        }

    protected:
        glm::vec3 pos;
        glm::vec3 scale;
        std::shared_ptr<Material> material;
        float theta;
        glm::vec3 axis;
};


#endif