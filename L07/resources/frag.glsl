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
	vec3 h = normalize((normalize(lightPos.xyz-position.xyz) - (normalize(position.xyz)) ) / 2.0);

	vec3 ca = ka;
	vec3 cs = ks * pow(max(0.0, dot(h,n)), s);
	vec3 cd = kd * max(0.0 , dot(n.xyz, normalize(lightPos.xyz-position.xyz)));

	gl_FragColor = vec4(ca.r + cd.r + cs.r, ca.g + cd.g + cs.g, ca.b + cd.b + cs.b, 1.0);
}
