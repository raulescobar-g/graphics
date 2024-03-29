#version 120

uniform mat4 P;
uniform mat4 MV;
uniform mat4 iMV;
uniform float t;

attribute vec4 aPos; // In object space
attribute vec2 aTex;

varying vec3 vNor; // In camera space
varying vec2 vTex;

float f(in float _x) {
	return cos(_x) + 2.0;
}

float df(in float _x) {
	return -sin(_x);
}

void main() {

	float theta = aPos.y;
	float x = aPos.x;

	vec4 position = vec4(x, f(x + t) * cos(theta), f(x + t) * sin(theta), 1.0);
	
	vec3 pdx = vec3(1.0, df(x + t) * cos(theta), df(x + t) * sin(theta));
	vec3 pdt = vec3(0.0, -f(x + t) * sin(theta), f(x + t) * cos(theta));
	vec3 norm = cross(pdx, pdt);
	norm = normalize(norm);


	gl_Position = P * (MV * position);
	vNor = normalize(vec3(iMV * vec4(norm, 0.0))); // Assuming MV contains only translations and rotations
	vTex = aTex;
}
