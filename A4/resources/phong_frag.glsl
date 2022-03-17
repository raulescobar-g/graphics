#version 120

uniform vec3 lightPos1;
uniform vec3 l1;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec3 normal;
varying vec3 position;

void main()
{
	vec3 n = normalize(normal);
	vec3 ca = ka;

	vec3 h1 = normalize((normalize(lightPos1.xyz-position.xyz) - (normalize(position.xyz)) ));
	vec3 cs1 = ks * pow(max(0.0, dot(h1,n)), s);
	vec3 cd1 = kd * max(0.0 , dot(n.xyz, normalize(lightPos1.xyz-position.xyz)));

	float r = l1.r * (ca.r + cd1.r + cs1.r );
	float g = l1.g * (ca.g + cd1.g + cs1.g );
	float b = l1.b * (ca.b + cd1.b + cs1.b );
	gl_FragColor = vec4(r,g,b, 1.0);
}

