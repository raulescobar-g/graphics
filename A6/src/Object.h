#pragma once

#ifndef OBJECT_H
#define OBJECT_H

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Material.h"

class Hit;

class Object {
    public:
        Object(glm::vec3 pos, glm::vec3 scale, std::shared_ptr<Material> material, std::string _type="normal"): pos(pos), scale(scale), material(material), type(_type) {
            M = glm::scale( glm::translate(glm::mat4(1.0f), pos) , scale);
            iM = glm::inverse(M);
            Mt = glm::transpose(M);
        };

        virtual void intersections(glm::vec3 origin, glm::vec3 direction, std::vector<Hit>& hitBuff) {
            return;
        }
        virtual void intersections(glm::vec3 origin, glm::vec3 direction, std::vector<Hit>& hitBuff, float scalar) {
            return;
        }

        virtual std::shared_ptr<Material> get_material() { 
            return material;
        }
        virtual std::string get_type() {
            return type;
        }
        protected:
            glm::vec3 pos;
            glm::vec3 scale;
            std::shared_ptr<Material> material;
            glm::mat4 M;
            glm::mat4 iM;
            glm::mat4 Mt;
            std::string type;

};


#endif