#version 120

uniform sampler2D texture0;
uniform sampler2D texture1;

varying vec2 vTex0;
varying vec2 vTex1;

void main()
{
	vec4 color0 = texture2D(texture0, vTex0);
	vec4 color1 = texture2D(texture1, vTex1);
	gl_FragColor = color1.r > 0.96 && color1.g > 0.96 && color1.b > 0.96 ? color0 : color1;
}
