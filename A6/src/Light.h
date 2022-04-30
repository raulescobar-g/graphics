#pragma once

#ifndef LIGHT_H
#define LIGHT_H

#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Light {
    public:
        Light(glm::vec3 position, float intensity) : position(position), intensity(intensity) {};

        glm::vec3 get_position() { return position; };

        float get_intensity() { return intensity; };

    private:
        glm::vec3 position;
        float intensity;
};


#endif