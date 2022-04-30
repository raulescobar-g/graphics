#pragma  once
#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class MatrixStack;

class Camera
{
public:
	
	Camera();
	virtual ~Camera();
	void setAspect(float a) { aspect = a; };
	void applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const;
	void applyOrthoMatrix(std::shared_ptr<MatrixStack> P,float w, float h) const;
	void applyOrthoTopMatrix(std::shared_ptr<MatrixStack> P,float w, float h) const;
	void applyViewMatrix(std::shared_ptr<MatrixStack> MV) const;
	void applyTopViewMatrix(std::shared_ptr<MatrixStack> MV) const;

	float y() const;
	void y(float);
	void dy(float);

	void dpos(glm::vec3);

	void increment_fovy();
	void decrement_fovy();

	float get_yaw() const;
	float get_pitch() const;
	void set_pitch(float);

	void dyaw(float);
	void dpitch(float);

	void move(bool,bool,bool,bool,float);

private:
	float aspect;
	float fovy;
	float znear;
	float zfar;
	float height;
	glm::vec3 pos;
	float yaw;
	float pitch;
};

#endif
