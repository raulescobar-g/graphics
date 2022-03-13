#version 120

uniform mat4 P;
uniform mat4 MV;
uniform mat4 iMV;
uniform mat3 T;

attribute vec4 aPos;
attribute vec3 aNor;
attribute vec2 aTex;

varying vec2 cloudPos;
varying vec2 vTex0;
varying vec3 norm;
varying vec3 pos;

void main()
{
	gl_Position = P * MV * aPos;
	vTex0 = aTex;

	cloudPos = (T * vec3(aTex,1.0)).xy;

	pos = (MV * aPos).xyz;
	norm = normalize(iMV * vec4(aNor,0.0)).xyz;

	
}
