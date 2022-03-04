#version 120

uniform mat4 P;
uniform mat4 MV;

attribute vec4 aPos; // in object space
attribute vec3 aNor; // in object space

varying vec4 position;
varying vec4 normal;


void main()
{
	gl_Position = P * MV * aPos;

	position = MV * aPos;

	normal =  MV * vec4(aNor,0.0);
	
}
