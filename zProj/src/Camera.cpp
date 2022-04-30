#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "MatrixStack.h"

#define FOVY_SPEED 0.01f
#define MOVEMENT_SPEED 5.0f


Camera::Camera() :
	aspect(1.0f),
	fovy((float)(45.0*M_PI/180.0)),
	znear(0.1f),
	zfar(1000.0f),
	height(1.0f),
	pos(0.0f,height,0.0f),
	yaw(glm::pi<float>()),
	pitch(0.0f)
	{}

Camera::~Camera(){}

void Camera::applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const{
	P->multMatrix(glm::perspective(fovy, aspect, znear, zfar));
}

void Camera::applyOrthoMatrix(std::shared_ptr<MatrixStack> P,float w, float h) const{
	P->multMatrix(glm::ortho(-w/2.0f, w/2.0f, -h/2.0f, h/2.0f,znear,zfar));
}

void Camera::applyOrthoTopMatrix(std::shared_ptr<MatrixStack> P,float w, float h) const{
	P->multMatrix(glm::ortho(-w/15.0f, w/15.0f, -h/15.0f, h/15.0f,znear,zfar));
}

void Camera::applyViewMatrix(std::shared_ptr<MatrixStack> MV) const{
	MV->multMatrix(glm::lookAt(pos, pos + glm::normalize(glm::vec3(sin(yaw)*cos(pitch), sin(pitch), cos(yaw)*cos(pitch))), glm::vec3(0.0,1.0,0.0) ));
}

void Camera::applyTopViewMatrix(std::shared_ptr<MatrixStack> MV) const{
	glm::vec3 camera_pos = glm::vec3(50.0f,100.0f,50.0f);
	MV->multMatrix(glm::lookAt(camera_pos, camera_pos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0,0.0,0.0) ));
}

void Camera::increment_fovy() {
	fovy += FOVY_SPEED;
	if (fovy > 114.0f * glm::pi<float>()/180.0f) fovy = 114.0f * glm::pi<float>()/180.0f;
}
	
void Camera::decrement_fovy() {
	fovy -= FOVY_SPEED;
	if (fovy < 4.0f * glm::pi<float>()/180.0f) fovy = 4.0f * glm::pi<float>()/180.0f;
}

void Camera::dyaw(float dy) {
	yaw += dy;
}
void Camera::dpitch(float dp){
	pitch += dp;
}

float Camera::get_pitch() const {
	return pitch;
}
float Camera::get_yaw() const {
	return yaw;
}

void Camera::set_pitch(float new_pitch) {
	pitch = new_pitch;
}

void Camera::dpos(glm::vec3 dp){
	pos += dp;
}

float Camera::y() const{
	return pos.y;
}
void Camera::dy(float _dy) {
	pos.y += _dy;
}
void Camera::y(float _y){
	pos.y = _y;
}

void Camera::move(bool w, bool a, bool s, bool d, float dt){
	glm::vec3 buf(0.0f,0.0f,0.0f);

	glm::vec3 forward = glm::normalize(glm::vec3(sin(yaw)*cos(pitch) , 0.0f, cos(yaw)*cos(pitch)));
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3( sin(yaw)*cos(pitch) , 0.0f, cos(yaw)*cos(pitch)), glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 still(0.0f);

	buf += w ? forward : still;
	buf += s ? -forward : still;
	buf += a ? -right : still;
	buf += d ? right : still;

	if (glm::length(buf) > 0.00001f || glm::length(buf) < -0.00001f) { pos += glm::normalize(buf) * MOVEMENT_SPEED * dt; } // keeps the movement the same speed even if moving diagonally by summing direction of movement vectors and normalizing

}