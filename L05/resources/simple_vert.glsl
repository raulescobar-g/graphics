#version 120

uniform mat4 P;
attribute vec3 vertPos;
attribute vec3 rainbow;

varying vec3 color;

void main()
{
	gl_Position = P * vec4(vertPos, 1.0);
	color = rainbow;
}
