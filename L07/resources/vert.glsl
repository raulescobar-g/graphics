#version 120

uniform mat4 P;
uniform mat4 MV;

attribute vec4 aPos; // in object space
attribute vec3 aNor; // in object space


void main()
{
	gl_Position = P * MV * aPos;
}
