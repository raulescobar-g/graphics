#version 120

uniform vec3 lightPos[10]; //edit amount later
uniform vec3 lightCol[10]; //edit amount later
uniform vec3 ke;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec3 normal;
varying vec3 position;



void main()
{
	float A0 = 1.0;
	float A1 = 0.0429;
	float A2 = 0.9857;
	vec3 n = normalize(normal);
	

	vec3 color = ke;

	for (int i = 0; i < 10; ++i){
		float r = distance(lightPos[i].xyz, position.xyz);
		vec3 l = normalize(lightPos[i].xyz - position.xyz);
		vec3 h = normalize((l - (normalize(position.xyz)) ));

		vec3 diffuse = kd * max(0.0, dot(n,l));
		vec3 specular = ks * max(0.0, pow( dot(n,h),s ) );
		vec3 temp = lightCol[i] * (diffuse + specular);

		float attenuation = 1.0 / (A0 + A1 * r + A2 * r * r);

		color += temp * attenuation;
	}

	gl_FragColor = vec4(color, 1.0);
}

