#version 120

uniform mat4 P;
uniform mat4 MV;
attribute vec4 aPos; // In object space
attribute vec3 aNor; // In object space
attribute vec2 aTex;

varying vec3 vNor; // In camera space
varying vec2 vTex;

void main()
{
	gl_Position = P * (MV * aPos);
	vNor = vec3(MV * vec4(aNor, 0.0)); // Assuming MV contains only translations and rotations
	vTex = aTex;
}
