#version 120

uniform vec3 lightPos;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

// varying vec3 color; // passed from the vertex shader
varying vec3 vertex;
varying vec3 position;

void main()
{

	vec3 n = normalize(normal);
	vec3 color = 0.5 * (n + 1.0);
	gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
}
