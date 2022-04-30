#pragma  once
#ifndef CAMERA_H
#define CAMERA_H

#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class Camera
{
public:
	
	Camera(int w, int h) {
        width = w;
        height = h;
        aspect = (float)w / (float)h;
        fov = glm::pi<float>()/4.0f;
        pos = glm::vec3(0.0f,0.0f,5.0f);
    };

	virtual ~Camera() {}
    glm::vec3 get_pos() const { return pos;}
    int get_height() const {return height;};
    int get_width() const {return width;};
    int get_fov() const{return fov;};
	
private:
    int height;
    int width;
	float aspect;
	float fov;
	glm::vec3 pos;	
};

#endif