#version 120

uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 l1;
uniform vec3 l2;

uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec3 normal;
varying vec3 position;

void main()
{
    vec3 cs1,cd1,cs2,cd2;

	vec3 n = normalize(normal);
    vec3 light1 = normalize(lightPos1.xyz-position.xyz);
    vec3 light2 = normalize(lightPos2.xyz-position.xyz);

    if (dot(n,light1) + dot(n,light2) < 2 * 0.25) {
        gl_FragColor = vec4(0.0,0.0,0.0, 1.0);
        return;
    }  else if (dot(n,light1) + dot(n,light2) < 2 * 0.5) {

        cs1 = ks * pow(0.5, s);
        cd1 = kd * 0.5;

        cs2 = ks * pow(0.5, s);
        cd2 = kd * 0.5;

    } else if (dot(n,light1) + dot(n,light2) < 2 * 0.75) {

        cs1 = ks * pow(0.75, s);
        cd1 = kd * 0.75;

        cs2 = ks * pow(0.75, s);
        cd2 = kd * 0.75;

    } else if (dot(n,light1) + dot(n,light2) < 2 * 0.95) {

        cs1 = ks * pow(0.99, s);
        cd1 = kd * 0.99;

        cs2 = ks * pow(0.99, s);
        cd2 = kd * 0.99;

    } else {

        cs1 = ks * pow(1.0, s);
        cd1 = kd * 1.2;

        cs2 = ks * pow(1.0, s);
        cd2 = kd * 1.2;

    }
	vec3 ca = ka;

	float r = l1.r * (ca.r + cd1.r + cs1.r ) + l2.r * (ca.r + cd2.r + cs2.r );
	float g = l1.g * (ca.g + cd1.g + cs1.g ) + l2.g * (ca.g + cd2.g + cs2.g );
	float b = l1.b * (ca.b + cd1.b + cs1.b ) + l2.b * (ca.b + cd2.b + cs2.b );
	gl_FragColor = vec4(r,g,b, 1.0);
}

