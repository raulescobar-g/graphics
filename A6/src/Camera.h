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
        pitch = 0.0f;
        yaw = glm::pi<float>();
    };

	virtual ~Camera() {}
    glm::vec3 get_pos() const { return pos;}
    int get_height() const {return height;};
    int get_width() const {return width;};
    float get_fov() const{return fov;};
    float get_pitch(){return pitch; };
    float get_yaw(){return yaw; }

    void set_fov(float new_fov) {
        fov = new_fov * glm::pi<float>() / 180.0f;
    }

    void set_pos(glm::vec3 new_pos) {
        pos = new_pos;
    }

    void rotate(float _pitch, float _yaw){
        pitch +=  (glm::pi<float>() * _pitch)/180.0f;
        yaw +=  (glm::pi<float>() * _yaw )/180.0f;
    }
	
private:
    int height;
    int width;
	float aspect;
	float fov;
	glm::vec3 pos;	
    float pitch;
    float yaw;
};

#endif