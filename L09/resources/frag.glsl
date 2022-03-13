#version 120

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightPosCam;

varying vec2 vTex0;
varying vec2 cloudPos;
varying vec3 norm;
varying vec3 pos;

void main()
{
	vec3 l = normalize(normalize(lightPosCam.xyz) - normalize(pos));
	vec3 n = normalize(norm);
	vec3 eye = normalize(-1 * pos);
	vec3 h = normalize(l + eye);

	vec3 clouds = texture2D(texture2, cloudPos).rgb;

	vec3 kd = texture2D(texture0, vTex0).rgb * max(0.0, dot(l,n));
	vec3 ks = texture2D(texture1, vTex0).rgb * pow(max(0, dot(h,n)) ,50);

	gl_FragColor = vec4(kd+ks+clouds, 1.0);
}
