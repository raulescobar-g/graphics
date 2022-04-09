#version 120

uniform mat4 P;
uniform mat4 MV;
uniform mat4 iMV;
uniform float t;

attribute vec4 aPos; // In object space

varying vec3 normal; // In camera space
varying vec3 position;

float f(in float _x) {
	return cos(_x) + 2.0;
}

float df(in float _x) {
	return -sin(_x);
}

void main() {

	float theta = aPos.y;
	float x = aPos.x;

	position = (MV * vec4(x, f(x + t) * cos(theta), f(x + t) * sin(theta),1.0)).xyz;
	
	vec3 pdx = vec3(1.0, df(x + t) * cos(theta), df(x + t) * sin(theta));
	vec3 pdt = vec3(0.0, -f(x + t) * sin(theta), f(x + t) * cos(theta));
	vec3 norm = cross(pdx, pdt);

	gl_Position = P * (vec4(position,1.0));
	normal = normalize(vec3(iMV * vec4(normalize(norm), 0.0))); 
}
