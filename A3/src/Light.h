#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Light {
    public:
        Light(std::string name,std::string cname, glm::vec3 position, glm::vec3 color) : pos_name(name), color_name(cname), position(position), color(color){};

        glm::vec3 position;
        glm::vec3 color;
        std::string pos_name;
        std::string color_name;
};


#endif