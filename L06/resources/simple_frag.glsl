#version 120

uniform int dotx;
uniform int doty;
varying vec3 color;

void main()
{
	vec2 middle = vec2(dotx, doty);
	vec2 pos = vec2(gl_FragCoord.x, gl_FragCoord.y);

	float ds = distance(pos, middle);
	if (ds < 20.0) {
		discard;
	}
	else {
		
		gl_FragColor = vec4(color,1.0);
		gl_FragColor.r += ds/600.0;
		gl_FragColor.g += ds/600.0;
		gl_FragColor.b += ds/600.0;
	}
}
