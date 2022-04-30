#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#define LIGHT_AMOUNT 1


class Light {
    public:
        Light(std::string name, std::string color_n) : pos_name(name),color_name(color_n) {
            for (int i = 0; i < LIGHT_AMOUNT ; ++i){
                position[i] = glm::vec3((rand()%10)-5, ((rand()%4) + 40) / 10.0f, (rand()%10)-5);
                color[i] = glm::vec3((rand()%100) / 100.0f,(rand()%100) / 100.0f,(rand()%100) / 100.0f);
                world_positions[i] = glm::vec3(0.0f,0.0f,0.0f);
            }
        };
        std::string pos_name;
        std::string color_name;
        glm::vec3 position[LIGHT_AMOUNT];
        glm::vec3 world_positions[LIGHT_AMOUNT];
        glm::vec3 color[LIGHT_AMOUNT];
        
};


#endif