#version 120

uniform vec3 lightPos;
uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec4 normal;
varying vec4 position;

void main()
{
	vec3 n = normalize(normal.xyz);
	vec3 h = normalize((normalize(lightPos.xyz-position.xyz) - (normalize(position.xyz)) ));
	vec3 ca = ka;
	vec3 cs = ks * pow(max(0.0, dot(h,n)), s);
	vec3 cd = kd * max(0.0 , dot(n.xyz, normalize(lightPos.xyz-position.xyz)));

	float r = ca.r + cs.r + cd.r;
	float g = ca.g + cs.g + cd.g;
	float b = ca.b + cs.b + cd.b;
	gl_FragColor = vec4(r,g,b, 1.0);
}
