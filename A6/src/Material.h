#pragma  once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material {
    public:
        Material(glm::vec3 kd, glm::vec3 ks, glm::vec3 ka, float s, std::string _type="normal",float _util=20): kd(kd), ks(ks), ka(ka), s(s), type(_type), util(_util) {};
        Material(): kd(0.0f),ks(0.0f), ka(0.0f),s(0.0f) {};
        
        glm::vec3 kd;
        glm::vec3 ks;
        glm::vec3 ka;
        float s;
        std::string type;
        float util;
};
#endif
