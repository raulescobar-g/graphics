#version 120

uniform sampler2D textureA;
uniform sampler2D textureB;

varying vec2 vTex;

void main()
{
	gl_FragColor.rgb = texture2D(textureA, vTex).rgb;
}
